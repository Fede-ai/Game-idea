#include "homestate.hpp"
#include "consts.hpp"
#include <iostream>
#include <vector>

/* 
	Todo:
	- titolo
	- new game btn
	- load btn
	- join session
	- create session btn
	- settings btn (window.close)
	- exit btn
*/

HomeState::HomeState(sf::RenderWindow& inWindow)
	:
	window(inWindow)
{
	window.setView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(Consts::VIEW_SIZE_X, Consts::VIEW_SIZE_Y)));

	font.loadFromFile("fonts/Roboto-Regular.ttf");

	title.setFont(font);
	title.setStyle(sf::Text::Bold);
	title.setFillColor(sf::Color(245,245,221));
	title.setString(Consts::GAME_NAME);
	title.setCharacterSize(60);
	title.setOrigin(title.getLocalBounds().width/2, 0);
	title.setPosition(0, -400);

	textureBg.loadFromFile("textures/home_bg.png");
	spriteBg.setTexture(textureBg);
	spriteBg.setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);
	spriteBg.setOrigin(spriteBg.getLocalBounds().width / 2, spriteBg.getLocalBounds().height / 2);

	for (int i = 0; i < 6; i++) {
		buttons[i].setSize(sf::Vector2f(500, 100));
		buttons[i].setFillColor(sf::Color::White);

		buttonsText[i].setFont(font);
		buttonsText[i].setFillColor(sf::Color(139,68, 17));
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
				buttonsText[i].setString("JOIN SESSION");
				break;
			case 3:
				buttonsText[i].setString("CREATE SESSION");
				break;
			case 4:
				buttonsText[i].setString("SETTINGS");
				break;
			case 5:
				buttonsText[i].setString("EXIT");
				break;
		}


		buttonsText[i].setOrigin(buttonsText[i].getLocalBounds().width / 2, 0);
		buttons[i].setOrigin(buttons[i].getLocalBounds().width / 2, 0);

		if (i % 2 == 0) {
			buttonsText[i].setPosition(-350, -160 + 200.f * (i / 2));
			buttons[i].setPosition(-350, -200 + 200.f * (i / 2));
		} 
		else {
			buttonsText[i].setPosition(350, -160 + 200.f * (i / 2));
			buttons[i].setPosition(350, -200 + 200.f * (i / 2));
		}
	}
}

int HomeState::update(std::vector<sf::Event> events)
{
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	int whatHappened = 0;

	for (const auto& e : events) {
		if (e.type == sf::Event::MouseButtonPressed && e.key.code == sf::Mouse::Left){
			for (auto& b : buttons) {
				if (b.getGlobalBounds().contains(mousePos))
					b.setFillColor(sf::Color(245, 245, 221));
			}
		}
		else if (e.type == sf::Event::MouseButtonReleased && e.key.code == sf::Mouse::Left) {
			for (int i = 0; i < 6; i++) {
				buttons[i].setFillColor(sf::Color::White);
				if (buttons[i].getGlobalBounds().contains(mousePos))
					whatHappened = handleClick(i);
			}
			
		}
	}

	return whatHappened;
}

void HomeState::draw()
{
	window.clear(sf::Color(102, 57, 49));
	window.draw(spriteBg);

	window.draw(title);

	for (int i = 0; i < 6; i++) {
		window.draw(buttons[i]);
		window.draw(buttonsText[i]);
	}


	window.display();
}

int HomeState::handleClick(int buttonId)
{
	switch (buttonId) {
		case 0:
			return 1;
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			window.close();
			break;
	}
	return 0;
}
