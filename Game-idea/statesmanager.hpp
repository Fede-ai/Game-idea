#pragma once
#include "gamestate.hpp"
#include "homestate.hpp"
#include <chrono>

class StatesManager {
public:
	StatesManager();
	int run();

private:
	Settings settings;
	GameInfo gameInfo;
	sf::RenderWindow window;
	State* state = NULL;
	sf::Cursor cursor;
	bool isTransitioning = false;
};

