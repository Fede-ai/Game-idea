#include "gamestate.hpp"
#include <iostream>

GameState::GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo, Settings inSettings)
	:
	window(inWindow),
	gameInfo(inGameInfo),
	settings(inSettings)
{	
	resourcesTexture[0].loadFromFile("textures/wood.png");
	resourcesTexture[1].loadFromFile("textures/stone.png");
	resourcesTexture[2].loadFromFile("textures/people.png");
	resourcesTexture[3].loadFromFile("textures/gold.png");
	resourcesTexture[4].loadFromFile("textures/food.png");
	resourcesTexture[5].loadFromFile("textures/water.png");
	for (int i = 0; i < 6; i++) {
		resourcesSprite[i].setTexture(resourcesTexture[i]);
		resourcesSprite[i].setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);
	}

	grassTexture.loadFromFile("textures/grass.png");
	grassSprite.setTexture(grassTexture);
	grassSprite.setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);

	resourcesBgTexture.loadFromFile("textures/resources.png");
	resourcesBgSprite.setTexture(resourcesBgTexture);
	resourcesBgSprite.setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);
	resourcesBgSprite.setOrigin(resourcesBgSprite.getLocalBounds().width, 0);
	
	lastMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
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

	//draw resources icons
	for (int i = 0; i < 3; i++) {
		resourcesSprite[2 * i].setPosition(center + sf::Vector2f(270 + 215 * i, -510));
		window.draw(resourcesSprite[2 * i]);
		resourcesSprite[2 * i + 1].setPosition(center + sf::Vector2f(270 + 215 * i, -440));
		window.draw(resourcesSprite[2 * i + 1]);
	}

	window.display();
}
