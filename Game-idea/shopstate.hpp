#pragma once
#include "state.hpp"
#include <iostream>
#include "consts.hpp"
#include <string>
#include "weapons.hpp"
#include "SFML/Network.hpp"

class ShopState : public State {
public:
	ShopState(sf::RenderWindow& inWindow, WeaponsManager& inWeaponsManager);
	~ShopState() {}

	int update(std::vector<sf::Event> events, float dTime);
	void draw();

private:
	int buttonClicked(int buttonId);
	WeaponsManager& weaponsManager;
	float deltaCards = 0;
	float deltaMin = 0;
	float btnPressed = -1;

	//bg & menu
	sf::Texture menuTexture;
	sf::Sprite menuSprite;	
	sf::Texture categoriesBgTexture;
	sf::Sprite categoriesBgSprite;
	sf::RectangleShape cardsOverlay;

	sf::Texture exitTexture;
	sf::Sprite exitSprite;

	//currencies
	int goldAmount = 1000;
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
	sf::Texture buyClickedTexture;
	sf::Sprite buy;

	sf::Text weaponTitle;
	sf::Text weaponPrice;
	sf::Text weaponDetails;
	sf::Text weaponDetailValues;
};