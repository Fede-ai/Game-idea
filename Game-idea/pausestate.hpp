#pragma once
#include "state.hpp"

class PauseState : public IngameState {
public:
	PauseState(sf::RenderWindow& inWindow);

	int update(std::vector<sf::Event> events, float dTime);
	void draw();
	int setState();
private:
	int handleClick(int buttonId);

	//bg & menu
	sf::Texture menuTexture;
	sf::Sprite menuSprite;

	sf::RectangleShape shadow;

	sf::Texture textureBtn;
	sf::Texture textureBtnPressed;
	sf::Sprite buttons[6];
	sf::Text buttonsText[6];
	sf::Font font;
	sf::Text title;
	int buttonPressed = -1;
};