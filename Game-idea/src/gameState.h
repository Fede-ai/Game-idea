#pragma once
#include "state.h"
#include "consts.h"
#include "building.h"
#include "resource.h"

struct GameInfo
{
	GameInfo();
	int typeBuilding = -1;
	int lastBuilding = -1;
	std::vector<Building> buildings;
	std::vector<Resource> resources;
	sf::View gameView = sf::View(sf::Vector2f(0, 0), sf::Vector2f(1920*9, 1080*9));
	//sf::View gameView = sf::View(sf::Vector2f(0, 0), Consts::viewSize);
	int seed = Consts::random(0, 1'000'000);

	int nWood = 0;
	int nStone = 0;
	int nGold = 0;
	int nGem = 0;
	int population = 0;

	int day = 0;
	const int maxTime = 5 * 60;
	int time = 0; //in secs
	size_t lastTime; //in secs
	bool isDay = true;
};

class GameState : public State
{
public:
	GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo, Textures& inTextures);
	void handleEvents();
	int update();
	void draw();

private:
	void updateWallsTextures();
	bool canPlaceBuilding(Building building);

	void spawnResources(int x, int y);	
	bool canSpawnResource(Resource resource);

	sf::Vector2f lastMousePos;
	sf::Vector2i chunkTl;
	
	sf::Texture grassTexture;
	sf::Sprite grassSprite;

	sf::RectangleShape preview;
	sf::Sprite shop;
	
	GameInfo& info;
	Textures& textures;
};