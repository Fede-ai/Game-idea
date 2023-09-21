#pragma once
#include "state.h"
#include "consts.h"
#include "building.h"

struct GameInfo
{
	int typeBuilding = -1;
	std::vector<Building> buildings;
	sf::View gameView = sf::View(sf::Vector2f(0, 0), Consts::viewSize);
};

class GameState : public State
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
	sf::RectangleShape shop;

	GameInfo& info;
};