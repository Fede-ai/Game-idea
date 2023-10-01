#include "resource.h"

Resource::Resource(Textures& textures, int inType, sf::Vector2f inPos)
{
	type = inType;
	pos = sf::Vector2i(inPos.x / Consts::cellSize, inPos.y / Consts::cellSize);

	body.setScale(Consts::pixelSize, Consts::pixelSize);
	body.setPosition(inPos.x, inPos.y);

	if (type == 0)
		body.setTexture(textures.wood);
	else if (type == 1)
		body.setTexture(textures.stone);
	else if (type == 2)
		body.setTexture(textures.gold);
	else if (type == 3)
		body.setTexture(textures.gem);

	hitbox.width = size[type].x;
	hitbox.height = size[type].y;
	hitbox.top = pos.y;
	hitbox.left = pos.x;

	spawnHitbox.width = size[type].x + 2*spawnDistance;
	spawnHitbox.height = size[type].y + 2*spawnDistance;
	spawnHitbox.top = pos.y - spawnDistance;
	spawnHitbox.left = pos.x - spawnDistance;
}

void Resource::draw(sf::RenderWindow& window)
{
	window.draw(body);
}

const sf::Vector2i Resource::size[4] = {
	sf::Vector2i(6, 4), //wood
	sf::Vector2i(4, 4), //stone
	sf::Vector2i(3, 3), //gold
	sf::Vector2i(2, 3) //gem
};