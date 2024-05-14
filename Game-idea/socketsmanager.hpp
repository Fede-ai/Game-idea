#pragma once
#include <SFML/Network.hpp>
#include <stack>
#include "consts.hpp"

class SocketsManager : sf::NonCopyable {
public:
	//should be called in a separate thread
	void connect();

	bool isVersionCompatible();
	bool isConnected();

	bool pollTcpPacket(sf::Packet& p);
	bool pollUdpPacket(sf::Packet& p);

	void sendTcpPacket(sf::Packet p);
	void sendUdpPacket(sf::Packet p);

private:
	void receiveTcp();
	void receiveUdp();

	sf::Mutex mutex;

	sf::TcpSocket tcpServer;
	bool isUdpRunning = false;
	sf::UdpSocket udpServer;

	bool version = true;
	bool connected = false;

	std::stack<sf::Packet> tcpPackets;
	std::stack<sf::Packet> udpPackets;
};

