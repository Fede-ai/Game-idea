#pragma once
#include <SFML/Network.hpp>

struct Client {
	sf::TcpSocket* tcp = NULL;
	sf::Uint16 port = 0;
	sf::IpAddress ip = "";

	sf::Vector2<sf::Int64> pos = { 0, 0 };
};