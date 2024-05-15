#pragma once
#include "gameinfo.hpp"
#include "settings.hpp"
#include "pausestate.hpp"
#include "state.hpp"
#include "socketsmanager.hpp"

class GameState : public State {
public:
	GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo, Settings inSettings, SocketsManager& inSocketsManager);
	~GameState();

private:
	int update(std::vector<sf::Event> events, float dTime);
	void draw();
	
	SocketsManager& socketsManager;
	IngameState* ingameState = NULL;
	GameInfo& gameInfo;
	const Settings settings;
	sf::Font font;

	sf::Texture grassTexture;
	sf::Sprite grassSprite;
	sf::Texture bodyTexture;
	sf::Sprite bodySprite;
};

