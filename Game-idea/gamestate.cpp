#include "gamestate.hpp"
#include <iostream>

GameState::GameState(sf::RenderWindow* inWindow)
	:
	window(inWindow)
{
}

int GameState::update(std::vector<sf::Event> events)
{
	for (const auto& e : events) {
		if (e.type == sf::Event::Closed)
			window->close();
	}

	return 0;
}

void GameState::draw()
{
	window->clear(sf::Color::Black);

	window->display();
}
