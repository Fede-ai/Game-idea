#pragma once
#include <SFML/Network.hpp>

class Server {
public:
	Server();
	int run();

private:
	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*> clients;
};

