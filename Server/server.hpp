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

	std::map<sf::Uint16, Client> clients;
	std::vector<sf::TcpSocket*> uninitialized;

	std::set<sf::Uint16> publicLobby;

	sf::Uint16 currentId = 0;
};

