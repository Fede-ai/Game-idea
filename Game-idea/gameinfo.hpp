#pragma once
#include "consts.hpp"
#include <SFML/Graphics.hpp>

struct GameInfo {

	sf::View view = sf::View(sf::Vector2f(0, 0), sf::Vector2f(Consts::VIEW_SIZE_X, Consts::VIEW_SIZE_Y));
};