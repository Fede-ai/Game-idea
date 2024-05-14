#pragma once
#include "gamestate.hpp"
#include "homestate.hpp"
#include "socketsmanager.hpp"

class StatesManager {
public:
	StatesManager();
	int run();

private:
	Settings settings;
	GameInfo gameInfo;
	SocketsManager socketsManager;

	sf::RenderWindow window;
	State* state = NULL;

	sf::Cursor cursor;
	bool isTransitioning = false;
};

//1 = connection approved
//2 = server version incompatibility