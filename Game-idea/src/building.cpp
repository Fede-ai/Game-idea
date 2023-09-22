#include "building.h"

Building::Building(Texture* textures, int inType, sf::Vector2f inPos)
{
	type = inType;
	pos = sf::Vector2i(inPos.x / Consts::cellSize, inPos.y / Consts::cellSize);
	
	body.setScale(Consts::pixelSize, Consts::pixelSize);
	body.setPosition(inPos.x, inPos.y - 3 * Consts::pixelSize);

	hitbox.setSize(sf::Vector2f(size[type].x * Consts::cellSize, size[type].y * Consts::cellSize));
	hitbox.setPosition(inPos);
}

void Building::draw(sf::RenderWindow& window)
{
	window.draw(body);
}

const sf::Vector2i Building::size[1] = {
	sf::Vector2i(1, 1)
};