#include "socketsmanager.hpp"
#include <thread>
#include <iostream>

SocketsManager::SocketsManager()
{
	std::thread connectThread(&SocketsManager::connect, this);
	connectThread.detach();
}

bool SocketsManager::isVersionCompatible()
{
	return version;
}
bool SocketsManager::isConnected()
{
	return connected;
}

bool SocketsManager::pollTcpPacket(sf::Packet& p)
{
	p.clear();

	mutex.lock();
	bool areThereOthers = tcpPackets.size() > 0;
	if (areThereOthers) {
		p = tcpPackets.front();
		tcpPackets.pop();
	}
	mutex.unlock();

	return areThereOthers;
}
bool SocketsManager::pollUdpPacket(sf::Packet& p)
{
	p.clear();

	mutex.lock();
	bool areThereOthers = udpPackets.size() > 0;
	if (areThereOthers) {
		p = udpPackets.front();
		udpPackets.pop();
	}
	mutex.unlock();

	return areThereOthers;
}

void SocketsManager::sendTcpPacket(sf::Packet p)
{
	if (!connected)
		return;

	auto s = tcpServer.send(p);

	if (s == sf::Socket::Disconnected)
		connected = false;
}
void SocketsManager::sendUdpPacket(sf::Packet p)
{
	if (!connected)
		return;

	udpServer.send(p, CON::SERVER_IP, CON::UDP_SERVER_PORT);
}

void SocketsManager::connect()
{
	if (!isUdpBinded) {
		if (udpServer.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
			//handle failed binding
			std::exit(-2);
		}
		isUdpBinded = true;
	}

	//construct a simple STUN binding request
	char request[20] = { 0 };
	request[0] = 0x00; //STUN message type: binding request
	request[1] = 0x01;
	request[2] = 0x00; //message Length: 0
	request[3] = 0x00;
	request[4] = 0x21; //magic Cookie
	request[5] = 0x12;
	request[6] = 0xA4;
	request[7] = 0x42;

	//random transaction ID
	srand(time(NULL));
	for (int i = 8; i < 20; ++i)
		request[i] = rand() % 256;

	if (udpServer.send(request, sizeof(request), "stun.l.google.com", 19302) != sf::Socket::Done)
		std::exit(-3);

	char response[64];
	std::size_t received;
	sf::IpAddress sender;
	unsigned short senderPort;
	if (udpServer.receive(response, sizeof(response), received, sender, senderPort) != sf::Socket::Done)
		std::exit(-4);

	unsigned short port = 0;
	//process the STUN response to get the public IP and port
	if (received >= 28 && response[0] == 0x01 && response[1] == 0x01)
		 port = ((response[26] << 8) | response[27]) ^ 0x2112;
	else
		std::exit(-5);

	std::cout << "local: " << udpServer.getLocalPort() << ", public: " << port << "\n";

	sf::Packet p;
	sf::Uint8 res = 0;
	while (res != TCP::REC::CONNECTED && res != TCP::REC::VERSION_INCOMPATIBLE) {
		while (tcpServer.connect(CON::SERVER_IP, CON::TCP_SERVER_PORT, sf::seconds(10)) != sf::Socket::Done)
			sf::sleep(sf::seconds(1));

		p.clear();
		p << TCP::SEND::CONNECT << std::string("dev1") << sf::Uint16(port);
		tcpServer.send(p);

		p.clear();
		if (tcpServer.receive(p) == sf::Socket::Done)
			p >> res;
	}

	if (res == TCP::REC::CONNECTED) {
		connected = true;
		std::thread tcpThread(&SocketsManager::receiveTcp, this);
		tcpThread.detach();
		if (!isUdpRunning) {
			std::thread udpThread(&SocketsManager::receiveUdp, this);
			udpThread.detach();
			isUdpRunning = true;
		}
	}
	else if (res == TCP::REC::VERSION_INCOMPATIBLE)
		version = false;
}

void SocketsManager::receiveTcp()
{
	while (connected) {
		sf::Packet p;
		auto s = tcpServer.receive(p);

		if (s == sf::Socket::Status::Done) {
			mutex.lock();
			tcpPackets.push(p);
			mutex.unlock();
		}
		else if (s == sf::Socket::Status::Disconnected)
			connected = false;
	}

	connect();
}
void SocketsManager::receiveUdp()
{
	while (true) {
		sf::IpAddress ip;
		unsigned short port;

		sf::Packet p;
		udpServer.receive(p, ip, port);

		if (ip == CON::SERVER_IP) {
			mutex.lock();
			udpPackets.push(p);
			mutex.unlock();
		}
	}
}