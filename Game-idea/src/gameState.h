#pragma once
#include "state.h"
#include "consts.h"
#include "building.h"

struct GameInfo
{
	int typeBuilding = -1;
	int lastBuilding = -1;
	std::vector<Building> buildings;
	sf::View gameView = sf::View(sf::Vector2f(Consts::fieldSize.x/2, Consts::fieldSize.y/2), Consts::viewSize);
};

class GameState : public State
{
public:
	GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo);
	void handleEvents();
	int update();
	void draw();

private:
	void sortBuildings();
	void setWallsTextures();
	sf::Vector2f lastMousePos;

	sf::Texture grassTexture;
	sf::Sprite grassSprite;

	sf::RectangleShape preview;
	bool canPosition = false;
	sf::Sprite shop;

	GameInfo& info;
	Texture* textures;
};