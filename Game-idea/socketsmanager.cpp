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
	if (areThereOthers > 0) {
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
	if (areThereOthers > 0) {
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

initConnection:

	sf::Packet p;
	sf::Uint8 res = 0;
	while (res != TCP::REC::CONNECTED && res != TCP::REC::VERSION_INCOMPATIBLE) {
		while (tcpServer.connect(CON::SERVER_IP, CON::TCP_SERVER_PORT, sf::seconds(10)) != sf::Socket::Done)
			sf::sleep(sf::seconds(2));

		p.clear();
		p << TCP::SEND::CONNECT << std::string("dev0");
		tcpServer.send(p);

		p.clear();
		if (tcpServer.receive(p) == sf::Socket::Done)
			p >> res;
	}

	if (res == TCP::REC::CONNECTED) {
		bool sendUdp = true;
		sf::Uint32 myId;
		p >> myId;

		auto initUdp = [this](sf::Uint32 id, bool& send) {
			while (send) {
				sf::Packet pac;
				pac << UDP::SEND::INIT_PORT << id;
				udpServer.send(pac, CON::SERVER_IP, CON::UDP_SERVER_PORT);

				sf::sleep(sf::seconds(2));
			}
		};
		std::thread initUdpThread(initUdp, myId, std::ref(sendUdp));

		p.clear();
		tcpServer.receive(p);
		p >> res;

		sendUdp = false;
		initUdpThread.join();

		if (res != TCP::REC::UDP_INITIALIZED)
			goto initConnection;

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

		if (ip == CON::SERVER_IP && port == CON::UDP_SERVER_PORT) {
			mutex.lock();
			udpPackets.push(p);
			mutex.unlock();
		}
	}
}