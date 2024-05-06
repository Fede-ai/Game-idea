#include "gamestate.hpp"
#include <iostream>

GameState::GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo, Settings inSettings)
	:
	State(inWindow),
	gameInfo(inGameInfo),
	settings(inSettings)
{
	grassTexture.loadFromFile("textures/grass.png");
	grassSprite.setTexture(grassTexture);
	grassSprite.setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);
	
	font.loadFromFile("fonts/PublicPixel.ttf");
	lastMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	window.setView(gameInfo.view);
}

int GameState::update(std::vector<sf::Event> events, float dTime)
{

	if (ingameState != NULL) {
		int whatHappened = ingameState->update(events, dTime);

		if (whatHappened == 1) {
			delete ingameState;
			ingameState = NULL;
		}

		return 0;
	}

	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	//handle events
	for (const auto& e : events) {
		if (e.type == sf::Event::Closed)
			window.close();
		else if (e.type == sf::Event::KeyReleased && e.key.code == sf::Keyboard::Escape)	
			ingameState = new PauseState(window);
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

	if (ingameState != NULL)
		ingameState->draw();
}
