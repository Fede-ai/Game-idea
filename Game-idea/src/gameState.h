#pragma once
#include "state.h"
#include "consts.h"
#include "building.h"

struct GameInfo
{
	int isPositioning = 0;
	std::vector<Building> buildings;
};

class GameState : public State, public GameInfo
{
public:
	GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo);
	void handleEvents();
	int update();
	void draw();

private:
	sf::Vector2f lastMousePos;

	sf::Texture grassTexture;
	sf::Sprite grassSprite;

	sf::RectangleShape preview;
};