#pragma once
#include "lobby.hpp"

class Server {
public:
	Server();
	int run();

private:
	sf::SocketSelector selector;
	sf::TcpListener listener;
	sf::UdpSocket udp;

	std::map<sf::Uint32, Client> clients;
	sf::Uint32 currentId = 0;
	std::vector<sf::TcpSocket*> uninitialized;

	Lobby publicLobby;
};

