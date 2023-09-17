#include "gameState.h"

GameState::GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo)
	:
	State(inWindow),
	gameInfo(inGameInfo)
{
}

int GameState::update()
{

	return 0;
}

void GameState::draw()
{
}
