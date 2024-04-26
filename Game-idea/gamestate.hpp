#pragma once
#include "state.hpp"

class GameState : public State
{
public:
	GameState(sf::RenderWindow* inWindow);

private:
	int update(std::vector<sf::Event> events);
	void draw();

	sf::RenderWindow* window;
};

