#pragma once
#include <SFML/Graphics.hpp>
#include <random>

namespace Consts 
{
	static const sf::Vector2i windowSize = sf::Vector2i(sf::VideoMode::getDesktopMode().width*2/3, sf::VideoMode::getDesktopMode().width*3/8);
	static const sf::Vector2f viewSize = sf::Vector2f(1920, 1080);
	static const std::string gameName = "Game idea";	
	static const int pixelSize = 4; 
	static const int cellSize = pixelSize * 6; //pixel per cell = 6
	static const sf::Vector2f chunkSize = sf::Vector2f(viewSize.x * 3, viewSize.y * 3);
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
		grass.loadFromFile("texture/grass.png");

		wood.loadFromFile("texture/wood.png");
		stone.loadFromFile("texture/stone.png");
		gold.loadFromFile("texture/gold.png");
		gem.loadFromFile("texture/gem.png");

		woodFarm.loadFromFile("texture/woodFarm.png");
		stoneFarm.loadFromFile("texture/stoneFarm.png");
		goldFarm.loadFromFile("texture/goldFarm.png");
		gemFarm.loadFromFile("texture/gemFarm.png");

		woodIcon.loadFromFile("texture/woodIcon.png");
		stoneIcon.loadFromFile("texture/stoneIcon.png");
		goldIcon.loadFromFile("texture/goldIcon.png");
		gemIcon.loadFromFile("texture/gemIcon.png");
	}
	sf::Texture wall[16];
	sf::Texture grass;
	sf::Texture shop;

	sf::Texture wood;
	sf::Texture stone;
	sf::Texture gold;
	sf::Texture gem;

	sf::Texture woodFarm;
	sf::Texture stoneFarm;
	sf::Texture goldFarm;
	sf::Texture gemFarm;

	sf::Texture woodIcon;
	sf::Texture stoneIcon;
	sf::Texture goldIcon;
	sf::Texture gemIcon;
};
