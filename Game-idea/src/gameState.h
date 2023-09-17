#pragma once
#include "state.h"

struct GameInfo
{

};

class GameState : public State
{
public:
	GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo);
	int update();
	void draw();

private:
	GameInfo& gameInfo;
};