#pragma once
#include "gameinfo.hpp"
#include "settings.hpp"
#include "pausestate.hpp"
#include "state.hpp"
#include "socketsmanager.hpp"
#include "weapons.hpp"

class GameState : public State {
public:
	GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo, Settings inSettings, SocketsManager& inSocketsManager, WeaponsManager& inWeaponManager);
	~GameState();

private:
	int update(std::vector<sf::Event> events, float dTime);
	void draw();
	
	WeaponsManager& weaponManager;
	SocketsManager& socketsManager;
	IngameState* ingameState = NULL;
	GameInfo& gameInfo;
	LobbyInfo lobbyInfo;
	const Settings settings;
	sf::Font font;

	sf::Texture grassTexture;
	sf::Sprite grassSprite;
	sf::Texture bodyTexture;
	sf::Sprite bodySprite;

	size_t lastServerUpdate = 0;
};
