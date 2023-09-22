#pragma once
#include "consts.h"

class Building
{
public:
	Building(Texture* textures, int inType, sf::Vector2f inPos);
	void draw(sf::RenderWindow& window);

	static const sf::Vector2i size[1];
	sf::Sprite body;
	sf::RectangleShape hitbox;
	sf::Vector2i pos;

private:
	int type;
};