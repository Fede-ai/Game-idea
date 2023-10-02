#pragma once
#include "consts.h"

class Building
{
public:
	//position must be "extended" (actual drawing position)
	Building(Textures& textures, int inType, sf::Vector2f inPos);
	void draw(sf::RenderWindow& window);
	void setTextureNum(Textures& textures, int num);

	static const sf::Vector2i size[5];
	sf::IntRect hitbox;
	sf::Vector2i pos;
	int type;

private:
	sf::Sprite body;
};