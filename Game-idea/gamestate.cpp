#include "gamestate.hpp"
#include <iostream>

GameState::GameState(sf::RenderWindow& inWindow)
	:
	window(inWindow)
{
	grassTexture.loadFromFile("textures/grass.png");
	grassSprite.setTexture(grassTexture);

	lastMousePos = sf::Vector2f(sf::Mouse::getPosition(window));
	lastMousePos.x *= window.getView().getSize().x / window.getSize().x;
	lastMousePos.y *= window.getView().getSize().y / window.getSize().y;
}

int GameState::update(std::vector<sf::Event> events)
{
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
	mousePos.x *= window.getView().getSize().x / window.getSize().x;
	mousePos.y *= window.getView().getSize().y / window.getSize().y;
	auto mouseCoords = [mousePos, this]() {
		sf::Vector2f coords = mousePos + window.getView().getCenter();
		coords.x -= window.getView().getSize().x / 2;
		coords.y -= window.getView().getSize().y / 2;
		return coords;
	};

	//move view with right click
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		sf::View view = window.getView();
		view.move(lastMousePos - mousePos);
		window.setView(view);
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

	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			float xPos = (std::floor(window.getView().getCenter().x / 1120) + x) * 1120;
			float yPos = (std::floor(window.getView().getCenter().y / 630) + y) * 630;
			grassSprite.setPosition(xPos, yPos);
			window.draw(grassSprite);
		}
	}

	sf::CircleShape center(5);
	center.setOrigin(5, 5);
	center.setPosition(window.getView().getCenter());
	window.draw(center);

	window.display();
}
