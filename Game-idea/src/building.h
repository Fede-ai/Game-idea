#pragma once
#include "consts.h"

class Building
{
public:
	//position must be "extended" (actual drawing position)
	Building(Textures& textures, int inType, sf::Vector2f inPos);
	void draw(sf::RenderWindow& window);
	void setTextureNum(Textures& textures, int num);

	sf::Vector2i pos;
	sf::IntRect hitbox;
	int type;

	static const sf::Vector2i size[5];
	static const enum types
	{
		none = -1,
		wall = 0,
		woodFarm = 1,
		stoneFarm = 2,
		goldFarm = 3,
		gemFarm = 4
	};

private:	
	sf::Sprite body;
};