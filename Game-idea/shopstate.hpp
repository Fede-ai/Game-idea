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
	int goldAmount = 10000;
	sf::Texture goldTexture;
	sf::Sprite gold;

	sf::Font font;
	sf::Text textGold;

	//card
	sf::Texture cardTexture;
	sf::Sprite card;

	sf::Texture weaponTexture;
	sf::Sprite weapon;

	sf::Texture buyTexture;
	sf::Sprite buy;

	sf::Text weaponTitle;
	sf::Text weaponDetails;
	sf::Text weaponDetailValues;

};