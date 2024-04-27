#include "gamestate.hpp"
#include <iostream>

GameState::GameState(sf::RenderWindow* inWindow)
	:
	window(inWindow)
{
	grassTexture.loadFromFile("textures/grass.png");
	grassSprite.setTexture(grassTexture);

	lastMousePos = sf::Vector2f(sf::Mouse::getPosition(*window));
	lastMousePos.x *= window->getView().getSize().x / window->getSize().x;
	lastMousePos.y *= window->getView().getSize().y / window->getSize().y;
}

int GameState::update(std::vector<sf::Event> events)
{
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*window));
	mousePos.x *= window->getView().getSize().x / window->getSize().x;
	mousePos.y *= window->getView().getSize().y / window->getSize().y;

	for (const auto& e : events) {
		if (e.type == sf::Event::Closed)
			window->close();
		if (e.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			sf::View view = window->getView();
			view.move(lastMousePos - mousePos);
			window->setView(view);
		}
	}

	lastMousePos = mousePos;
	return 0;
}

void GameState::draw()
{
	window->clear(sf::Color::Black);

	window->draw(grassSprite);

	window->display();
}
