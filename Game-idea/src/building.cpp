#include "building.h"

Building::Building(Textures& textures, int inType, sf::Vector2f inPos)
{
	type = inType;
	pos = sf::Vector2i(inPos.x / Consts::cellSize, inPos.y / Consts::cellSize);
	
	body.setScale(Consts::pixelSize, Consts::pixelSize);
	body.setPosition(inPos.x, inPos.y - 3 * Consts::pixelSize);

	hitbox.width = size[type].x;
	hitbox.height = size[type].y;
	hitbox.top = pos.y;
	hitbox.left = pos.x;

	switch (type)
	{
	case 1:

		break;
	}
}

void Building::draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void Building::setTextureNum(Textures& textures, int num)
{
	body.setTexture(textures.wall[num]);
}

const sf::Vector2i Building::size[5] = {
	sf::Vector2i(1, 1), //wall
	sf::Vector2i(2, 2), //wood farm
	sf::Vector2i(2, 2), //stone farm
	sf::Vector2i(2, 2), //gold farm
	sf::Vector2i(2, 2) //gem famr
};