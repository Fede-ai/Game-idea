#include "pausestate.hpp"
#include "consts.hpp"
#include <iostream>

PauseState::PauseState(sf::RenderWindow& inWindow)
	: 
	IngameState(inWindow)
{
	//menu texture
	menuTexture.loadFromFile("textures/home_bg.png");
	menuSprite.setTexture(menuTexture);
	menuSprite.setScale(sf::Vector2f(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE));
	menuSprite.setOrigin(menuSprite.getLocalBounds().width / 2, menuSprite.getLocalBounds().height / 2);

	//shadow
	shadow.setSize(sf::Vector2f(Consts::VIEW_SIZE_X, Consts::VIEW_SIZE_Y));
	shadow.setFillColor(sf::Color(0, 0, 0, 70));
	shadow.setPosition(window.getView().getCenter()-sf::Vector2f(Consts::VIEW_SIZE_X / 2, Consts::VIEW_SIZE_Y / 2));

	//load font
	font.loadFromFile("fonts/PublicPixel.ttf");

	//button bg 
	textureBtn.loadFromFile("textures/button.png");
	textureBtnPressed.loadFromFile("textures/button_pressed.png");

	//home buttons
	buttonsText[0].setString("RESUME");
	buttonsText[1].setString("SHOP");
	buttonsText[2].setString("SETTINGS");
	buttonsText[3].setString("BANANA");
	buttonsText[4].setString("HOME");
	buttonsText[5].setString("EXIT");
	for (int i = 0; i < 6; i++) {
		//set buttons scale and texture
		buttons[i].setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);
		buttons[i].setTexture(textureBtn);

		//buttons' text, font and style
		buttonsText[i].setFont(font);
		buttonsText[i].setFillColor(sf::Color(66, 32, 26));
		buttonsText[i].setStyle(sf::Text::Bold);
		buttonsText[i].setCharacterSize(25);

		//set buttons origin
		buttonsText[i].setOrigin(buttonsText[i].getLocalBounds().width / 2, 0);
		buttons[i].setOrigin(buttons[i].getLocalBounds().width / 2, 0);

		//set buttons position
		float x = 300.f * ((i % 2) * 2 - 1), y = -250, dist = 200.f * int(i / 2.f);
		buttonsText[i].setPosition(x, y + dist);
		buttons[i].setPosition(x, y + dist - 30);
	}
}

int PauseState::update(std::vector<sf::Event> events, float dTime)
{
	//handle events
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	int whatHappened = 0;
	for (const auto& e : events) {
		if (e.type == sf::Event::Closed)
			window.close();
		else if (e.type == sf::Event::KeyReleased && e.key.code == sf::Keyboard::Escape)
			return 1;
		//handle left pressed
		else if (e.type == sf::Event::MouseButtonPressed && e.key.code == sf::Mouse::Left) {
			for (int i = 0; i < 6; i++) {
				//button pressed animation
				if (buttons[i].getGlobalBounds().contains(mousePos)) {
					buttons[i].setTexture(textureBtnPressed);
					buttonsText[i].setPosition(buttonsText[i].getPosition().x - 10, buttonsText[i].getPosition().y + 10);
					buttonPressed = i;
				}
			}
		}
		//handle left released
		else if (e.type == sf::Event::MouseButtonReleased && e.key.code == sf::Mouse::Left) {
			for (int i = 0; i < 6; i++) {
				//clicked button action
				if (buttons[i].getGlobalBounds().contains(mousePos) && buttonPressed == i)
					whatHappened = handleClick(i);
				//finish button click animation
				buttons[i].setTexture(textureBtn);
			}

			//finish text clicked animation
			if (buttonPressed != -1)
				buttonsText[buttonPressed].setPosition(buttonsText[buttonPressed].getPosition().x + 10, buttonsText[buttonPressed].getPosition().y - 10);
			buttonPressed = -1;
		}
	}

	return whatHappened;
}

void PauseState::draw()
{
	window.draw(shadow);
	window.draw(menuSprite);
	for (int i = 0; i < 6; i++) {
		window.draw(buttons[i]);
		window.draw(buttonsText[i]);
	}

	//do NOT call window.display()
}

int PauseState::handleClick(int buttonId)
{
	switch (buttonId) {
	case 0:
		return 1; // RESUME
		break;
	case 4: 
		return 2; // HOME
		break;
	case 5: 
		return 3; // EXIT
		break;
	}
	return 0;
}

