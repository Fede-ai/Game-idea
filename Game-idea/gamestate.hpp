#pragma once
#include "consts.hpp"
#include "state.hpp"

class GameState : public State
{
public:
	GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo, Settings inSettings);

private:
	int update(std::vector<sf::Event> events);
	void draw();

	sf::RenderWindow& window;
	GameInfo& gameInfo;
	const Settings settings;

	sf::Vector2f lastMousePos;

	sf::Texture grassTexture;
	sf::Sprite grassSprite;
	sf::Texture resourcesTexture;
	sf::Sprite resourcesSprite;
};

