#include "homestate.hpp"
#include "consts.hpp"
#include <iostream>
#include <vector>

HomeState::HomeState(sf::RenderWindow& inWindow)
	:
	State(inWindow)
{
	window.setView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(Consts::VIEW_SIZE_X, Consts::VIEW_SIZE_Y)));

	//load font
	font.loadFromFile("fonts/PublicPixel.ttf");

	//set title settings
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
	spriteSettings.setPosition(sf::Vector2f(float(- Consts::VIEW_SIZE_X / 2 + 60), float(Consts::VIEW_SIZE_Y / 2 - 60)));
	//close icon
	textureClose.loadFromFile("textures/close.png");
	spriteClose.setTexture(textureClose);
	spriteClose.setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);
	spriteClose.setOrigin(spriteClose.getLocalBounds().width / 2, spriteClose.getLocalBounds().height / 2);
	spriteClose.setPosition(sf::Vector2f(float(Consts::VIEW_SIZE_X / 2 - 60), float(Consts::VIEW_SIZE_Y / 2 - 60)));

	//button bg 
	textureBtn.loadFromFile("textures/button.png");
	textureBtnPressed.loadFromFile("textures/button_pressed.png");

	//home buttons
	buttonsText[0].setString("NEW GAME");
	buttonsText[1].setString("LOAD GAME");
	buttonsText[2].setString("NEW SESSION");
	buttonsText[3].setString("JOIN SESSION");
	for (int i = 0; i < 4; i++) {
		//set buttons scale and texture
		buttons[i].setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);
		buttons[i].setTexture(textureBtn);

		//buttons' text, font and style
		buttonsText[i].setFont(font);
		buttonsText[i].setFillColor(sf::Color(66,32, 26));
		buttonsText[i].setStyle(sf::Text::Bold);
		buttonsText[i].setCharacterSize(25);

		//set buttons origin
		buttonsText[i].setOrigin(buttonsText[i].getLocalBounds().width / 2, 0);
		buttons[i].setOrigin(buttons[i].getLocalBounds().width / 2, 0);

		//set buttons position
		int x = 300 * ((i % 2) * 2 - 1), y = -90, dist = 200 * int(i / 2);
		buttonsText[i].setPosition(x, y + dist);
		buttons[i].setPosition(x, y + dist - 30);
	}
}

int HomeState::update(std::vector<sf::Event> events, float dTime)
{
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	int whatHappened = 0;

	for (const auto& e : events) {
		// when X is clicked
		if (e.type == sf::Event::Closed) 
			window.close();
		
		//handle left pressed
		else if (e.type == sf::Event::MouseButtonPressed && e.key.code == sf::Mouse::Left) {
			for (int i = 0; i < 4; i++) {
				//button pressed animation
				if (buttons[i].getGlobalBounds().contains(mousePos)) {
					buttons[i].setTexture(textureBtnPressed);
					buttonsText[i].setPosition(buttonsText[i].getPosition().x - 10, buttonsText[i].getPosition().y + 10);
					buttonPressed = i;
				}		
			}
			//settings and Exit icon pressed
			if (spriteSettings.getGlobalBounds().contains(mousePos))
				buttonPressed = 4;
			else if (spriteClose.getGlobalBounds().contains(mousePos))
				buttonPressed = 5;
		}
		//handle left released
		else if (e.type == sf::Event::MouseButtonReleased && e.key.code == sf::Mouse::Left) {
			for (int i = 0; i < 4; i++) {
				//clicked button action
				if (buttons[i].getGlobalBounds().contains(mousePos) && buttonPressed == i)
					whatHappened = handleClick(i);	
				//finish button click animation
				buttons[i].setTexture(textureBtn);
			}

			//finish text clicked animation
			if (buttonPressed > -1 && buttonPressed < 4) 
				buttonsText[buttonPressed].setPosition(buttonsText[buttonPressed].getPosition().x + 10, buttonsText[buttonPressed].getPosition().y - 10);

			//clicked button action for Settings and Exit
			if (spriteSettings.getGlobalBounds().contains(mousePos) && buttonPressed == 4)
				whatHappened = handleClick(4);
			else if (spriteClose.getGlobalBounds().contains(mousePos) && buttonPressed == 5)
				whatHappened = handleClick(5);

			buttonPressed = -1;
		}
	}	

	//on mouse hover rotates settings icon
	if (spriteSettings.getGlobalBounds().contains(mousePos))
		spriteSettings.rotate(float(-0.1 * dTime));

	return whatHappened;
}

void HomeState::draw()
{
	window.clear(sf::Color(102, 57, 49));
	window.draw(spriteBg);

	//draw 4 action buttons 
	for (int i = 0; i < 4; i++) {
		window.draw(buttons[i]);
		window.draw(buttonsText[i]);
	}

	//draw title, settings and Exit
	window.draw(title);
	window.draw(spriteSettings);
	window.draw(spriteClose);

	//DO NOT CALL window.display()
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
