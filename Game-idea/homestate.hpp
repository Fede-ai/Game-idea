#pragma once
#include "state.hpp"

class HomeState : public State {
public:
	HomeState(sf::RenderWindow& inWindow);
	~HomeState() {}

private:
	int update(std::vector<sf::Event> events, float dTime);
	void draw();
	int handleClick(int buttonId);

	sf::Texture bgTexture;
	sf::Sprite bgSprite;

	sf::Texture textureBtn;
	sf::Texture textureBtnPressed;
	sf::Sprite buttons[4];
	sf::Text buttonsText[4];
	sf::Font font;
	sf::Text title;
	int buttonPressed = -1;

	sf::Texture textureMenu;
	sf::Sprite spriteMenu;

	sf::Texture textureSettingsBG;
	sf::Sprite spriteSettingsBG;

	sf::Texture textureSettings;
	sf::Sprite spriteSettings;
	sf::Texture textureClose;
	sf::Sprite spriteClose;

	sf::Texture textureNotification;
	sf::Sprite spriteNotification;
	sf::Text notificationText;
	sf::RectangleShape shadow;
};