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
	sf::RectangleShape itemBg;
	int nItems = 5;
	const int itemPerRow = 4;
	const int space = 60;
};
