#include "shopState.h"

ShopState::ShopState(sf::RenderWindow& inWindow, GameInfo& inGameInfo)
	:
	State(inWindow),
	info(inGameInfo)
{
}

void ShopState::handleEvents()
{
}

int ShopState::update()
{
	
	return status;
}

void ShopState::draw()
{
	window.clear(sf::Color(sf::Color(80, 80, 80)));
}
