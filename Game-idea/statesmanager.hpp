#pragma once
#include "gamestate.hpp"
#include "homestate.hpp"
#include "shopstate.hpp"
#include "weapons.hpp"

class StatesManager {
public:
	StatesManager();
	~StatesManager();
	int run();

private:
	void handleTcp();

	Settings settings;
	GameInfo gameInfo;
	WeaponsManager weaponManager;
	Server server;

	sf::RenderWindow window;
	State* state = NULL;

	sf::Cursor cursor;
	bool isTransitioning = false;
};

//1 = connection approved
//2 = server version incompatibility