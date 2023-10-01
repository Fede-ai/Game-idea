#pragma once
#include "consts.h"

class Resource
{
public:
	Resource(Textures& textures, int inType, sf::Vector2f inPos);
	void draw(sf::RenderWindow& window);

	static const sf::Vector2i size[4];
	sf::IntRect hitbox;
	sf::IntRect spawnHitbox;
	sf::Vector2i pos;
	int type;

private:
	sf::Sprite body;
	static const int spawnDistance = 4;
};