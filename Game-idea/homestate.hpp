#pragma once
#include "state.hpp"

class HomeState : public State {
public:
	HomeState(sf::RenderWindow& inWindow);

private:
	int update(std::vector<sf::Event> events);
	void draw();

	sf::RenderWindow& window;

};