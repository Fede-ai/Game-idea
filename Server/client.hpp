#pragma once
#include <SFML/Network.hpp>

struct Client {
	sf::TcpSocket* tcp = NULL;
	sf::Uint16 port = 0;
};