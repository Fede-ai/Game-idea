#pragma once
#include "state.hpp"
#include <iostream>

class ShopState : public IngameState {
public:
	ShopState(sf::RenderWindow& inWindow);
	~ShopState() {}

	int update(std::vector<sf::Event> events, float dTime);
	void draw();

private:
	//bg & menu
	sf::Texture menuTexture;
	sf::Sprite menuSprite;
};