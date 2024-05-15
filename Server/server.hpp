#pragma once
#include <SFML/Network.hpp>
#include "client.hpp"
#include <set>

class Server {
public:
	Server();
	int run();

private:
	void handleUdp();

	sf::TcpListener listener;
	sf::SocketSelector selector;
	sf::UdpSocket udp;

	std::map<int, Client> clients;
	std::vector<sf::TcpSocket*> uninitialized;

	std::set<int> publicLobby;

	int currentId = 0;
};

