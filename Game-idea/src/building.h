#pragma once
#include "consts.h"

class Building
{
public:
	Building(int inType, sf::Vector2f pos);
	void draw(sf::RenderWindow& window);

	static const sf::Vector2i size[1];

private:
	int type;
	sf::RectangleShape body;
};