#include "settingsstate.hpp"

SettingsState::SettingsState(sf::RenderWindow& inWindow, Settings& inSettings)
	:
	State(inWindow),
	settings(inSettings)
{
}

int SettingsState::update(std::vector<sf::Event> events, float dTime)
{
	return 0;
}

void SettingsState::draw()
{
	window.clear();

	//DO NOT CALL window.display()
}
