#include "pausestate.hpp"

PauseState::PauseState(sf::RenderWindow& inWindow)
	:
	State(inWindow)
{
}

int PauseState::update(std::vector<sf::Event> events, float dTime)
{
	return 0;
}

void PauseState::draw()
{
	window.clear();

	//DO NOT CALL window.display()
}
