#pragma once
#include "gameinfo.hpp"
#include "settings.hpp"
#include "state.hpp"

class GameState : public State {
public:
	GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo, Settings inSettings);

private:
	int update(std::vector<sf::Event> events, float dTime);
	void draw();
	
	IngameState* inGameState = NULL;
	GameInfo& gameInfo;
	const Settings settings;
	sf::Font font;
	sf::Vector2f lastMousePos;

	sf::Texture grassTexture;
	sf::Sprite grassSprite;
};

