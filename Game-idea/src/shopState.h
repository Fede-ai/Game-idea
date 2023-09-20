#pragma once
#include "gameState.h"

class ShopState : public State
{
public:
	ShopState(sf::RenderWindow& inWindow, GameInfo& inGameInfo);
	void handleEvents();
	int update();
	void draw();

private:
	GameInfo& info;
};
