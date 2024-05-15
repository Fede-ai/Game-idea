#pragma once
#include <SFML/Network.hpp>
#include "client.hpp"

class Server {
public:
	Server();
	int run();

private:
	sf::TcpListener listener;
	sf::SocketSelector selector;
	sf::UdpSocket udp;

	std::map<int, Client> clients;
	std::vector<sf::TcpSocket*> uninitialized;

	int currentId = 0;
};

