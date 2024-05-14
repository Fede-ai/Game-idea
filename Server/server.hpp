#pragma once
#include <SFML/Network.hpp>

class Server {
public:
	Server();
	int run();

private:
	void acceptClients();

	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*> clients;
	sf::UdpSocket udp;
};

