#pragma once
#include <SFML/Graphics.hpp>

namespace Consts 
{
	static const sf::Vector2i windowSize = sf::Vector2i(sf::VideoMode::getDesktopMode().width*2/3, sf::VideoMode::getDesktopMode().width*3/8);
	static const sf::Vector2i fieldSize = sf::Vector2i(1920 * 5, 1080 * 5);
	static const sf::Vector2f viewSize = sf::Vector2f(1920, 1080);
	static const std::string gameName = "Game idea";
	static const int cellSize = 30;
	static const int pixelSize = 5;
}

struct Texture
{
	Texture(){
		wall.loadFromFile("texture/wall.png");
		shop.loadFromFile("texture/shop.png");
	}
	sf::Texture wall;
	sf::Texture shop;
};
