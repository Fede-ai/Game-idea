#pragma once
#include <SFML/Network.hpp>

struct Client {
	sf::TcpSocket* tcp = NULL;
	sf::Uint16 port = 0;
	sf::IpAddress ip = "";
	sf::Vector2f pos = { 0, 0 };
};