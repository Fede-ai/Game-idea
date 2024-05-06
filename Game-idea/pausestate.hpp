#pragma once
#include "state.hpp"

class PauseState : public State {
public:
	PauseState(sf::RenderWindow& inWindow);

private:
	int update(std::vector<sf::Event> events, float dTime);
	void draw();
};