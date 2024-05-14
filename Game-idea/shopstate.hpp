#pragma once
#include "state.hpp"
#include <iostream>
#include "consts.hpp"

class ShopState : public State {
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