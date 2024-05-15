#pragma once
#include "consts.hpp"
#include <SFML/Graphics.hpp>

struct GameInfo {
	//-32'000 -> 32'000
	sf::Vector2<sf::Int16> chunk;
	//-32'000 -> 32'000
	sf::Vector2<sf::Int16> pos;
};