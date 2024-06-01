#include "homestate.hpp"
#include "consts.hpp"
#include <iostream>
#include <vector>

HomeState::HomeState(sf::RenderWindow& inWindow, GameInfo& inGameInfo, Server& inServer)
	:
	State(inWindow),
	gameInfo(inGameInfo),
	server(inServer)
{
	window.setView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(CON::VIEW_WIDTH, CON::VIEW_HEIGHT)));

	//load font
	font.loadFromFile("fonts/PublicPixel.ttf");

	//set title settings
	title.setFont(font);
	title.setStyle(sf::Text::Bold);
	title.setFillColor(sf::Color(66, 32, 26));
	title.setString(CON::GAME_NAME);
	title.setCharacterSize(70);
	title.setOrigin(title.getLocalBounds().width/2, 0);
	title.setPosition(0, -300);

	//bg texture
	bgTexture.loadFromFile("textures/background.png");
	bgSprite.setTexture(bgTexture);
	bgSprite.setScale(CON::PIXEL_SIZE, CON::PIXEL_SIZE);
	bgSprite.setOrigin(bgSprite.getLocalBounds().width / 2, bgSprite.getLocalBounds().height / 2);

	//home bg texture
	textureMenu.loadFromFile("textures/home_bg.png");
	spriteMenu.setTexture(textureMenu);
	spriteMenu.setScale(CON::PIXEL_SIZE, CON::PIXEL_SIZE);
	spriteMenu.setOrigin(spriteMenu.getLocalBounds().width / 2, spriteMenu.getLocalBounds().height / 2);

	//settings icon
	textureSettings.loadFromFile("textures/settings.png");
	spriteSettings.setTexture(textureSettings);
	spriteSettings.setScale(CON::PIXEL_SIZE, CON::PIXEL_SIZE);
	spriteSettings.setOrigin(spriteSettings.getLocalBounds().width / 2, spriteSettings.getLocalBounds().height / 2);
	spriteSettings.setPosition(sf::Vector2f(float(- CON::VIEW_WIDTH / 2 + 70), float(CON::VIEW_HEIGHT / 2 - 70)));
	//settings bg
	textureSettingsBG.loadFromFile("textures/settings_bg.png");
	spriteSettingsBG.setTexture(textureSettingsBG);
	spriteSettingsBG.setScale(CON::PIXEL_SIZE, CON::PIXEL_SIZE);
	spriteSettingsBG.setOrigin(spriteSettingsBG.getLocalBounds().width / 2, spriteSettingsBG.getLocalBounds().height / 2);
	spriteSettingsBG.setPosition(sf::Vector2f(spriteSettings.getPosition().x, spriteSettings.getPosition().y));

	//close icon
	textureClose.loadFromFile("textures/close.png");
	spriteClose.setTexture(textureClose);
	spriteClose.setScale(CON::PIXEL_SIZE, CON::PIXEL_SIZE);
	spriteClose.setOrigin(spriteClose.getLocalBounds().width / 2, spriteClose.getLocalBounds().height / 2);
	spriteClose.setPosition(sf::Vector2f(float(CON::VIEW_WIDTH / 2 - 70), float(CON::VIEW_HEIGHT / 2 - 70)));

	//button bg 
	textureBtn.loadFromFile("textures/button.png");
	textureBtnPressed.loadFromFile("textures/button_pressed.png");

	//texture notification
	textureNotification.loadFromFile("textures/notification.png");
	spriteNotification.setTexture(textureNotification);
	spriteNotification.setScale(CON::PIXEL_SIZE, CON::PIXEL_SIZE);
	spriteNotification.setOrigin(spriteNotification.getLocalBounds().width / 2, spriteNotification.getLocalBounds().height / 2);

	shadow.setSize(sf::Vector2f(CON::VIEW_WIDTH, CON::VIEW_HEIGHT));
	shadow.setFillColor(sf::Color(0, 0, 0, 150));
	shadow.setPosition(window.getView().getCenter() - sf::Vector2f(CON::VIEW_WIDTH / 2, CON::VIEW_HEIGHT / 2));

	notificationText.setFont(font);
	notificationText.setFillColor(sf::Color(66, 32, 26));
	notificationText.setStyle(sf::Text::Bold);
	notificationText.setCharacterSize(25);
	notificationText.setString("Please update the game :(");
	notificationText.setOrigin(notificationText.getLocalBounds().width / 2, 20);

	//Connection status connectionStatus
	connectionStatus.setSize(sf::Vector2f(20, 20));
	connectionStatus.setPosition(sf::Vector2f(-CON::VIEW_WIDTH / 2 + 50, -CON::VIEW_HEIGHT / 2 + 50));

	//home buttons
	buttonsText[0].setString("PLAY");
	buttonsText[1].setString("SHOP");
	buttonsText[2].setString("PRIVATE");
	buttonsText[3].setString("JOIN");

	for (int i = 0; i < 4; i++) {
		//set buttons scale and texture
		buttons[i].setScale(CON::PIXEL_SIZE, CON::PIXEL_SIZE);
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
		float x = 300.f * ((i % 2) * 2 - 1), y = -90, dist = 200.f * int(i / 2.f);
		buttonsText[i].setPosition(x, y + dist);
		buttons[i].setPosition(x, y + dist - 30);
	}
}

int HomeState::update(std::vector<sf::Event> events, float dTime)
{
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	int whatHappened = 0;

	for (const auto& e : events) {
		//when X is clicked
		if (e.type == sf::Event::Closed) 
			window.close();

		if (!server.versionCompatible)
			continue;
		
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

	connectionStatus.setFillColor(server.isConnected ? sf::Color::Green : sf::Color::Red);
	if (!server.versionCompatible)
		return whatHappened;

	//manage tcp packets
	while (!server.packets.empty()) {
		sf::Packet p = server.packets.front();
		sf::Uint8 code;
		p >> code;

		//client joined public lobby
		if (code == REC::JOIN_PUBLIC) {
			whatHappened = 1;
			break;
		}

		server.packets.pop();
	}

	//on mouse hover rotates settings icon
	if (spriteSettings.getGlobalBounds().contains(mousePos))
		spriteSettings.rotate(float(-0.1 * dTime));

	return whatHappened;
}

void HomeState::draw()
{
	window.clear();
	window.draw(bgSprite);
	window.draw(spriteMenu);

	//draw 4 action buttons 
	for (int i = 0; i < 4; i++) {
		window.draw(buttons[i]);
		window.draw(buttonsText[i]);
	}

	//draw title, settings and Exit
	window.draw(title);
	window.draw(spriteSettingsBG);
	window.draw(spriteSettings);
	window.draw(spriteClose);
	window.draw(connectionStatus);

	if (!server.versionCompatible) {
		window.draw(shadow);
		window.draw(spriteNotification);
		window.draw(notificationText);
	}

	//do NOT call window.display()
}

int HomeState::handleClick(int buttonId)
{
	sf::Packet p;
	switch (buttonId) {
	case 0: //new game
		p << SEND::JOIN_PUBLIC;
		server.tcp.send(p);
		break;
      
	case 1: //shop
		return 3;
		break;

	case 2: //private
		break;

	case 3: //join
		break;

	case 4: //setting
		break;

	case 5: //close
		window.close();
		break;
	}
	return 0;
}
