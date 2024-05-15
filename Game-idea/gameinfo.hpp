#pragma once
#include "consts.hpp"
#include <SFML/Graphics.hpp>

struct Player {
	sf::Vector2f pos;
};

struct GameInfo {
	Player player;
	std::map<int, Player> otherPlayers;

	sf::Uint8 speed = 0;
	static constexpr sf::Uint8 speeds[] = { 4, 5, 6, 7 };
};