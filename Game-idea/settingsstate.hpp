#pragma once
#include "state.hpp"
#include "settings.hpp"

class SettingsState : public State {
public:
	SettingsState(sf::RenderWindow& inWindow, Settings& inSettings);

private:
	int update(std::vector<sf::Event> events, float dTime);
	void draw();

	Settings& settings;
};