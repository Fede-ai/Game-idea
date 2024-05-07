#pragma once
#include "gameinfo.hpp"
#include "settings.hpp"
#include "pausestate.hpp"
#include "state.hpp"

class GameState : public State {
public:
	GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo, Settings inSettings);
	~GameState();

private:
	int update(std::vector<sf::Event> events, float dTime);
	void draw();
	
	IngameState* ingameState = NULL;
	GameInfo& gameInfo;
	const Settings settings;
	sf::Font font;

	sf::Texture grassTexture;
	sf::Sprite grassSprite;
};

