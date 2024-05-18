#pragma once
#include "state.hpp"
#include <iostream>
#include "consts.hpp"
#include <string>

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

	//currencies
	int woodAmount = 10;
	int stoneAmount = 1;
	sf::Texture woodTexture;
	sf::Sprite wood;
	sf::Texture stoneTexture;
	sf::Sprite stone;

	sf::Font font;
	sf::Text textWood;
	sf::Text textStone;
};