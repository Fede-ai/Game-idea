#include "shopstate.hpp"

ShopState::ShopState(sf::RenderWindow& inWindow, GameInfo& inGameInfo)
	:
	State(inWindow),
	gameInfo(inGameInfo)
{
}

int ShopState::update(std::vector<sf::Event> events, float dTime)
{
	return 0;
}

void ShopState::draw()
{
	window.clear();

	//DO NOT CALL window.display()
}
