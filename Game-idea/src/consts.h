#pragma once
#include <SFML/Graphics.hpp>

namespace Consts {
	static const sf::Vector2i windowSize = sf::Vector2i(sf::VideoMode::getDesktopMode().width*2/3, sf::VideoMode::getDesktopMode().width*3/8);
	static const sf::Vector2i fieldSize = sf::Vector2i(1920 * 4, 1080 * 4);
	static const sf::Vector2f viewSize = sf::Vector2f(1920, 1080);
	static const std::string gameName = "Game idea";
	static const int pixelSize = 30;
}
