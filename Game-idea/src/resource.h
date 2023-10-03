#pragma once
#include "consts.h"
#include "building.h"

class Resource
{
public:
	Resource(Textures& textures, int inType, sf::Vector2f inPos);
	void draw(sf::RenderWindow& window);
	bool isBuildingInRange(Building building);

	sf::Vector2i pos;
	sf::IntRect hitbox;
	sf::IntRect spawnHitbox;
	int type;
	
	static const sf::Vector2i size[4];
	static const enum types {
		wood = 0,
		stone = 1,
		gold = 2,
		gem = 3
	};

private:	
	sf::Sprite body;
	sf::IntRect wideFarmHitbox;
	sf::IntRect tallFarmHitbox;

	static const int spawnDistance = 4;
	static const int farmDistance = 1;
};