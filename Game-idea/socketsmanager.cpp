#include "socketsmanager.hpp"
#include <thread>

void SocketsManager::connect()
{
	if (udpServer.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
		//handle failed binding
		std::exit(-2);
		return;
	}

	sf::Uint8 res = 0;
	while (res != 1 && res != 2) {
		while (tcpServer.connect(Consts::SERVER_IP, Consts::TCP_SERVER_PORT, sf::seconds(10)) != sf::Socket::Done)
			sf::sleep(sf::seconds(2));

		sf::Packet p;
		p << sf::Uint8(1) << std::string("dev") << sf::Uint16(udpServer.getLocalPort());
		tcpServer.send(p);

		p.clear();
		if (tcpServer.receive(p) == sf::Socket::Done)
			p >> res;
	}

	if (res == 2)
		version = false;
	else if (res == 1) {
		connected = true;

		std::thread tcpThread(&SocketsManager::receiveTcp, this);
		tcpThread.detach();

		if (!isUdpRunning) {
			std::thread udpThread(&SocketsManager::receiveUdp, this);
			udpThread.detach();
			isUdpRunning = true;
		}
	}
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
	if (tcpPackets.size() > 0) {
		p = tcpPackets.top();
		tcpPackets.pop();
	}
	bool areThereOthers = tcpPackets.size() > 0;
	mutex.unlock();

	return areThereOthers;
}
bool SocketsManager::pollUdpPacket(sf::Packet& p)
{
	p.clear();

	mutex.lock();
	if (udpPackets.size() > 0) {
		p = udpPackets.top();
		udpPackets.pop();
	}
	bool areThereOthers = udpPackets.size() > 0;
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

	auto s = udpServer.send(p, Consts::SERVER_IP, Consts::UDP_SERVER_PORT);
}

void SocketsManager::receiveTcp()
{
	sf::Socket::Status status = sf::Socket::Status::Done;
	while (status != sf::Socket::Status::Disconnected || !isConnected()) {
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
}
void SocketsManager::receiveUdp()
{
	while (true) {
		sf::IpAddress ip;
		unsigned short port;

		sf::Packet p;
		udpServer.receive(p, ip, port);

		if (ip == Consts::SERVER_IP && port == Consts::UDP_SERVER_PORT) {
			mutex.lock();
			tcpPackets.push(p);
			mutex.unlock();
		}
	}
}