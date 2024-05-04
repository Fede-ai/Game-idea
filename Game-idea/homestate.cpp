#include "homestate.hpp"
#include "consts.hpp"
#include <iostream>
#include <vector>

HomeState::HomeState(sf::RenderWindow& inWindow)
	:
	window(inWindow)
{
	window.setView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(Consts::VIEW_SIZE_X, Consts::VIEW_SIZE_Y)));

	font.loadFromFile("fonts/PublicPixel.ttf");

	title.setFont(font);
	title.setStyle(sf::Text::Bold);
	title.setFillColor(sf::Color(62, 39, 35));
	title.setString(Consts::GAME_NAME);
	title.setCharacterSize(70);
	title.setOrigin(title.getLocalBounds().width/2, 0);
	title.setPosition(0, -300);

	//home bg texture
	textureBg.loadFromFile("textures/home_bg.png");
	spriteBg.setTexture(textureBg);
	spriteBg.setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);
	spriteBg.setOrigin(spriteBg.getLocalBounds().width / 2, spriteBg.getLocalBounds().height / 2);

	//settings icon
	textureSettings.loadFromFile("textures/settings.png");
	spriteSettings.setTexture(textureSettings);
	spriteSettings.setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);
	spriteSettings.setOrigin(spriteSettings.getLocalBounds().width / 2, spriteSettings.getLocalBounds().height / 2);
	spriteSettings.setPosition(sf::Vector2f(- Consts::VIEW_SIZE_X/2 + spriteSettings.getGlobalBounds().width/1.5, + Consts::VIEW_SIZE_Y/2 - spriteSettings.getGlobalBounds().height/1.5));
	//close icon
	textureClose.loadFromFile("textures/close.png");
	spriteClose.setTexture(textureClose);
	spriteClose.setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);
	spriteClose.setOrigin(spriteClose.getLocalBounds().width / 2, spriteClose.getLocalBounds().height / 2);
	spriteClose.setPosition(sf::Vector2f(Consts::VIEW_SIZE_X / 2 - spriteClose.getGlobalBounds().width / 1.5, +Consts::VIEW_SIZE_Y / 2 - spriteClose.getGlobalBounds().height / 1.5));

	//button bg 
	textureBtn.loadFromFile("textures/button.png");
	textureBtnPressed.loadFromFile("textures/button_pressed.png");
	for (int i = 0; i < 4; i++) {
		buttons[i].setTexture(textureBtn);
		buttons[i].setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);
		buttons[i].setOrigin(buttons[i].getLocalBounds().width / 2, buttons[i].getLocalBounds().height / 2);
		buttons[i].setPosition(sf::Vector2f(Consts::VIEW_SIZE_X / 2 - buttons[i].getGlobalBounds().width / 1.5, +Consts::VIEW_SIZE_Y / 2 - buttons[i].getGlobalBounds().height / 1.5));
		buttons[i].setTexture(textureBtnPressed);

		buttonsText[i].setFont(font);
		buttonsText[i].setFillColor(sf::Color(66,32, 26));
		buttonsText[i].setStyle(sf::Text::Bold);
		buttonsText[i].setCharacterSize(25);
		buttonsText[i].setString(Consts::GAME_NAME);

		switch (i) {
			case 0:
				buttonsText[i].setString("NEW GAME");
				break;
			case 1:
				buttonsText[i].setString("LOAD GAME");
				break;
			case 2:
				buttonsText[i].setString("NEW SESSION");
				break;
			case 3:
				buttonsText[i].setString("JOIN SESSION");
				break;
		}

		buttonsText[i].setOrigin(buttonsText[i].getLocalBounds().width / 2, 0);
		buttons[i].setOrigin(buttons[i].getLocalBounds().width / 2, 0);

		if (i % 2 == 0) {
			buttonsText[i].setPosition(-350, -90 + 200.f * (i / 2));
			buttons[i].setPosition(-350, -120 + 200.f * (i / 2));
		}
		else {
			buttonsText[i].setPosition(350, -90 + 200.f * (i / 2));
			buttons[i].setPosition(350, -120 + 200.f * (i / 2));
		}
	}
}

int HomeState::update(std::vector<sf::Event> events, float dTime)
{
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	int whatHappened = 0;

	for (const auto& e : events) {
		if (e.type == sf::Event::Closed) {
			window.close();
		}
		//handle left pressed
		else if (e.type == sf::Event::MouseButtonPressed && e.key.code == sf::Mouse::Left) {
			for (int i = 0; i < 4; i++) {
				if (buttons[i].getGlobalBounds().contains(mousePos)) {
					buttons[i].setTexture(textureBtn);
					buttonsText[i].setPosition(buttonsText[i].getPosition().x - 10, buttonsText[i].getPosition().y + 10);
					buttonPressed = i;
				}		
			}
			if (spriteSettings.getGlobalBounds().contains(mousePos))
				buttonPressed = 4;
			else if (spriteClose.getGlobalBounds().contains(mousePos))
				buttonPressed = 5;
		}
		//handle left released
		else if (e.type == sf::Event::MouseButtonReleased && e.key.code == sf::Mouse::Left) {
			for (int i = 0; i < 4; i++) {
				if (buttons[i].getGlobalBounds().contains(mousePos) && buttonPressed == i)
					whatHappened = handleClick(i);					
				buttons[i].setTexture(textureBtnPressed);
			}
			if (buttonPressed > -1 && buttonPressed < 4)
				buttonsText[buttonPressed].setPosition(buttonsText[buttonPressed].getPosition().x + 10, buttonsText[buttonPressed].getPosition().y - 10);
			if (spriteSettings.getGlobalBounds().contains(mousePos) && buttonPressed == 4)
				whatHappened = handleClick(4);
			else if (spriteClose.getGlobalBounds().contains(mousePos) && buttonPressed == 5)
				whatHappened = handleClick(5);

			buttonPressed = -1;
		}
	}

	if (spriteSettings.getGlobalBounds().contains(mousePos))
		spriteSettings.rotate(-.1 * dTime);

	return whatHappened;
}

void HomeState::draw()
{
	window.clear(sf::Color(102, 57, 49));
	window.draw(spriteBg);
	
	for (int i = 0; i < 4; i++) {
		window.draw(buttons[i]);
		window.draw(buttonsText[i]);
	}

	window.draw(title);
	window.draw(spriteSettings);
	window.draw(spriteClose);

	window.display();
}

int HomeState::handleClick(int buttonId)
{
	switch (buttonId) {
		case 0: //new game
			return 1;
			break;
		case 1: //load game
			break;
		case 2: //new session
			break;
		case 3: //join session
			break;
		case 4: //setting
			break;
		case 5: //close
			window.close();
			break;
	}
	return 0;
}
