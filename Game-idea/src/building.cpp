#include "building.h"

Building::Building(int inType, sf::Vector2f pos)
{
	type = inType;
	body.setSize(sf::Vector2f(size[type].x * 6, size[type].y * 6));
	body.setPosition(pos);
}

void Building::draw(sf::RenderWindow& window)
{
	window.draw(body);
}

const sf::Vector2i Building::size[1] = {
	sf::Vector2i(4, 4)
};