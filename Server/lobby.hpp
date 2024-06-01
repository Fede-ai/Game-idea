#pragma once
#include "client.hpp"
#include <functional>

class Lobby {
public:
	Lobby(std::function<void(sf::Uint32, Client)> inLeaveLobby);
	void runLobby();
	void joinLobby(sf::Uint32 id, Client newClient);

private:
	void handleIncomingMsgs();

	char name[7] = "public";
	sf::SocketSelector selector;
	sf::UdpSocket udp;
	std::map<sf::Uint32, Client> players;
	std::function<void(sf::Uint32, Client)> leaveLobby;
};

