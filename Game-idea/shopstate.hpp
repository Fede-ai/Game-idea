#pragma once
#include "state.hpp"
#include "gameinfo.hpp"

class ShopState : public State {
public:
	ShopState(sf::RenderWindow& inWindow, GameInfo& inGameInfo);

private:
	int update(std::vector<sf::Event> events, float dTime);
	void draw();

	GameInfo& gameInfo;
};