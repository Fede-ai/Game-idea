#pragma once
#include "state.hpp"

class HomeState : public State {
public:
	HomeState(sf::RenderWindow& inWindow);

private:
	int update(std::vector<sf::Event> events);
	void draw();
	int handleClick(int buttonId);

	sf::RenderWindow& window;

	sf::RectangleShape buttons[6];
	sf::Text buttonsText[6];
	sf::Font font;
	sf::Text title;

	sf::Texture textureBg;
	sf::Sprite spriteBg;

	sf::Texture textureSettings;
	sf::Sprite spriteSettings;

	enum actions { 
		none = 0, 
		exit_program = 1
	};
};