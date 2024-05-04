#include "gamestate.hpp"
#include <iostream>

GameState::GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo, Settings inSettings)
	:
	State(inWindow),
	gameInfo(inGameInfo),
	settings(inSettings)
{
	for (int i = 0; i < 6; i++) {
		resourcesTexture[i].loadFromFile("textures/resources.png", sf::IntRect(sf::Vector2i(10 * i, 0), sf::Vector2i(10, 10)));
		resourcesSprite[i].setTexture(resourcesTexture[i]);
		resourcesSprite[i].setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);
	}

	grassTexture.loadFromFile("textures/grass.png");
	grassSprite.setTexture(grassTexture);
	grassSprite.setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);

	resourcesBgTexture.loadFromFile("textures/resourcesBg.png");
	resourcesBgSprite.setTexture(resourcesBgTexture);
	resourcesBgSprite.setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);
	resourcesBgSprite.setOrigin(resourcesBgSprite.getLocalBounds().width, 0);
	
	font.loadFromFile("fonts/PublicPixel.ttf");
	resourcesText.setFillColor(sf::Color::White);
	resourcesText.setFont(font);
	resourcesText.setCharacterSize(30);
	lastMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	window.setView(gameInfo.view);
}

int GameState::update(std::vector<sf::Event> events, float dTime)
{
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	//move view with right click
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		sf::View view = window.getView();
		view.move(lastMousePos - mousePos);
		view.setCenter(sf::Vector2f(round(view.getCenter().x), round(view.getCenter().y)));
		window.setView(view);

		mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	}

	//handle events
	for (const auto& e : events) {
		if (e.type == sf::Event::Closed)
			window.close();
	}

	lastMousePos = mousePos;
	return 0;
}

void GameState::draw()
{
	window.clear(sf::Color::Black);

	const sf::Vector2f center = window.getView().getCenter();
	const sf::FloatRect chunk = grassSprite.getGlobalBounds();
	//draw chunks
	for (int x = 0; x < 2; x++) {	
		float xChunk = std::floor(center.x / chunk.width) * chunk.width;
		if (int(center.x) % int(chunk.width) > chunk.width / 2 * center.x / abs(center.x))
			xChunk += chunk.width * x;
		else 
			xChunk -= chunk.width * x;
		
		for (int y = 0; y < 2; y++) {
			float yChunk = std::floor(center.y / chunk.height) * chunk.height;
			if (int(center.y) % int(chunk.height) > chunk.height / 2 * center.y / abs(center.y))
				yChunk += chunk.height * y;
			else
				yChunk -= chunk.height * y;

			grassSprite.setPosition(xChunk, yChunk);
			window.draw(grassSprite);
		}
	}

	resourcesBgSprite.setPosition(center + sf::Vector2f(window.getView().getSize().x / 2, -window.getView().getSize().y / 2));
	window.draw(resourcesBgSprite);

	auto valueToString = [] (int num, bool isCap = false) {
		if (num < 1'000) {
			std::string str = std::to_string(num);
			while (str.size() < 3 && !isCap)
				str = '0' + str;

			return str;
		}
		else if (num < 10'000)
			return std::to_string(num / 100) + "h";
		else if (num < 100'000)
			return std::to_string(num / 1000) + "k";
		else
			return std::string("err");
	};
	//draw resources icons
	for (int i = 0; i < 6; i++) {
		resourcesSprite[i].setPosition(center + sf::Vector2f(float(360 + 205 * int(i / 2)), float(70 * (i % 2) - 530)));
		window.draw(resourcesSprite[i]);

		resourcesText.setCharacterSize(30);
		resourcesText.setString(valueToString(int(gameInfo.resources[i])));
		resourcesText.setPosition(center + sf::Vector2f(float(445 + 205 * int(i / 2)), float(70 * (i % 2) - 532)));
		window.draw(resourcesText);
		resourcesText.setCharacterSize(15);
		resourcesText.setString("/" + valueToString(gameInfo.capacities[i], true));
		resourcesText.setPosition(center + sf::Vector2f(float(445 + 205 * int(i / 2)), float(70 * (i % 2) - 497)));
		window.draw(resourcesText);
	}

	//DO NOT CALL window.display()
}
