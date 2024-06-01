#pragma once
#include <SFML/Network.hpp>

struct Client {
	sf::TcpSocket* tcp = NULL;
	unsigned short port = 0;
	size_t timeLastMsg = 0;

	sf::Vector2<sf::Int64> pos = { 0, 0 };
};