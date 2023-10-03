#include "building.h"

Building::Building(Textures& textures, int inType, sf::Vector2f inPos)
{
	type = inType;
	pos = sf::Vector2i(inPos.x / Consts::cellSize, inPos.y / Consts::cellSize);
	
	body.setPosition(inPos);
	body.setScale(Consts::pixelSize, Consts::pixelSize);
	hitbox.width = size[type].x;
	hitbox.height = size[type].y;
	hitbox.top = pos.y;
	hitbox.left = pos.x;

	switch (type)
	{
	case types::wall:
		body.setPosition(body.getPosition().x, body.getPosition().y - 3 * Consts::pixelSize);
		break;
	case types::woodFarm:
		body.setTexture(textures.woodFarm);
		break;
	case types::stoneFarm:
		body.setTexture(textures.stoneFarm);
		break;
	case types::goldFarm:
		body.setTexture(textures.goldFarm);
		break;
	case types::gemFarm:
		body.setTexture(textures.gemFarm);
		break;
	}
}

void Building::draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void Building::setTextureNum(Textures& textures, int num)
{
	if (type == types::wall)
	{
		body.setTexture(textures.wall[num]);
	}
}

const sf::Vector2i Building::size[5] = {
	sf::Vector2i(1, 1), //wall
	sf::Vector2i(2, 2), //wood farm
	sf::Vector2i(2, 2), //stone farm
	sf::Vector2i(2, 2), //gold farm
	sf::Vector2i(2, 2) //gem famr
};