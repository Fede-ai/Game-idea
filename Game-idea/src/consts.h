#pragma once
#include <SFML/Graphics.hpp>
#include <random>

namespace Consts 
{
	static const sf::Vector2i windowSize = sf::Vector2i(sf::VideoMode::getDesktopMode().width*2/3, sf::VideoMode::getDesktopMode().width*3/8);
	static const sf::Vector2f viewSize = sf::Vector2f(1920, 1080);
	static const std::string gameName = "Game idea";
	static const int cellSize = 24; //pixel per cell = 6
	static const int pixelSize = 4; 
	static const sf::Vector2f chunkSize = sf::Vector2f(1920 * 3, 1080 * 3);
	static int random(int min, int max)
	{
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
		return dist6(rng);
	}
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
		wood.loadFromFile("texture/wood.png");
		stone.loadFromFile("texture/stone.png");
		gold.loadFromFile("texture/gold.png");
		gem.loadFromFile("texture/gem.png");
	}
	sf::Texture wall[16];
	sf::Texture shop;

	sf::Texture wood;
	sf::Texture stone;
	sf::Texture gold;
	sf::Texture gem;
};
