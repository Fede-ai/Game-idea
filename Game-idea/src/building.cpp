#include "building.h"

Building::Building(int inType, sf::Vector2f pos)
{
	type = inType;
	body.setSize(sf::Vector2f(size[type].x * Consts::pixelSize, size[type].y * Consts::pixelSize));
	body.setPosition(pos);
}

void Building::draw(sf::RenderWindow& window)
{
	window.draw(body);
}

const sf::Vector2i Building::size[1] = {
	sf::Vector2i(1, 1)
};