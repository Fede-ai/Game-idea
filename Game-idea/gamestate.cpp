#include "gamestate.hpp"
#include <iostream>

GameState::GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo, Settings inSettings)
	:
	window(inWindow),
	gameInfo(inGameInfo),
	settings(inSettings)
{
	grassTexture.loadFromFile("textures/grass.png");
	grassSprite.setTexture(grassTexture);
	grassSprite.setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);

	resourcesTexture.loadFromFile("textures/resources.png");
	resourcesSprite.setTexture(resourcesTexture);
	resourcesSprite.setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);
	resourcesSprite.setOrigin(resourcesSprite.getLocalBounds().width, 0);
	
	lastMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
}

int GameState::update(std::vector<sf::Event> events)
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

	resourcesSprite.setPosition(center + sf::Vector2f(window.getView().getSize().x / 2, -window.getView().getSize().y / 2));
	window.draw(resourcesSprite);

	window.display();
}
