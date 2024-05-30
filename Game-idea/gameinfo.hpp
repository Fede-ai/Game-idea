#pragma once
#include "consts.hpp"
#include <SFML/Graphics.hpp>

struct Player {
	sf::Vector2<sf::Int64> pos;
};

//reset every time you join a lobby
struct LobbyInfo {
	sf::Uint16 wood = 0;
	sf::Uint16 stone = 0;

	Player player;
	std::map<sf::Uint32, Player> otherPlayers;
	sf::Uint16 serverUdpPort;
};

//stored on the server
struct GameInfo {
	sf::Uint16 gold = 0;

	sf::Uint8 speed = 0;
	static constexpr sf::Uint8 speeds[] = { 4, 5, 6, 7 };
};