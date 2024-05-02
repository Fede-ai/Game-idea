#pragma once
#include "state.hpp"

class HomeState : public State {
public:
	HomeState(sf::RenderWindow& inWindow);

private:
	int update(std::vector<sf::Event> events, float dTime);
	void draw();
	int handleClick(int buttonId);

	sf::RenderWindow& window;

	sf::RectangleShape buttons[4];
	sf::Text buttonsText[4];
	sf::Font font;
	sf::Text title;
	int buttonPressed = -1;

	sf::Texture textureBg;
	sf::Sprite spriteBg;

	sf::Texture textureSettings;
	sf::Sprite spriteSettings;
	sf::Texture textureClose;
	sf::Sprite spriteClose;
};