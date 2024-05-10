#pragma once
#include "gamestate.hpp"
#include "homestate.hpp"

class StatesManager {
public:
	StatesManager();
	int run();

private:
	void handleUpdate(int code);
	void connectServer();

	Settings settings;
	GameInfo gameInfo;
	sf::RenderWindow window;
	State* state = NULL;
	sf::Cursor cursor;
	bool isTransitioning = false;

	sf::TcpSocket server;
	bool isConnected = false;
};

//1 = connection approved
//2 = server version incompatibility