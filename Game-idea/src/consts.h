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

struct Textures
{
	Textures() 
	{
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				sf::IntRect rect(sf::Vector2i(x*6, y*9), sf::Vector2i(6, 9));
				wall[x + 4 * y].loadFromFile("texture/wall.png", rect);
			}
		}
		shop.loadFromFile("texture/shop.png");
	}
	sf::Texture wall[16];
	sf::Texture shop;
};
