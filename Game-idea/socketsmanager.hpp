#pragma once
#include <SFML/Network.hpp>
#include <queue>
#include "consts.hpp"

class SocketsManager : sf::NonCopyable {
public:
	SocketsManager();

	bool isVersionCompatible();
	bool isConnected();

	bool pollTcpPacket(sf::Packet& p);
	bool pollUdpPacket(sf::Packet& p);

	void sendTcpPacket(sf::Packet p);
	void sendUdpPacket(sf::Packet p);

private:	
	//should be called in a separate thread
	void connect();

	void receiveTcp();
	void receiveUdp();

	sf::Mutex mutex;

	sf::TcpSocket tcpServer;
	sf::Uint16 id = 0;
	bool isUdpRunning = false;
	bool isUdpBinded = false;
	sf::UdpSocket udpServer;

	bool version = true;
	bool connected = false;

	std::queue<sf::Packet> tcpPackets;
	std::queue<sf::Packet> udpPackets;
};

