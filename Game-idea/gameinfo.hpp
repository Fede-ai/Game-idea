#pragma once
#include "consts.hpp"
#include <SFML/Graphics.hpp>

struct GameInfo {
	//wood, stone, people, gold, food, water
	double resources[6] = { 0, 0, 0, 0, 0, 0 };
	int capacities[6] = { 10, 10, 10, 10, 10, 10 };

	sf::View view = sf::View(sf::Vector2f(0, 0), sf::Vector2f(Consts::VIEW_SIZE_X, Consts::VIEW_SIZE_Y));
};