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

	//sf::View gameView = sf::View(sf::Vector2f(0, 0), sf::Vector2f(1920*9, 1080*9));
	sf::View gameView = sf::View(sf::Vector2f(0, 0), Consts::viewSize);
	long int seed = Consts::random(0, 1'000'000);

	float nResources[4] = {20, 5, 0, 0};
	int population = 0;
	int food = 0;

	int day = 0;
	const int dayTime = 5 * 60;
	const int nightTime = 3 * 60;
	int time = 0; //in secs
	size_t lastTime = 0; //in secs
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

	void drawCounter();

	sf::Vector2f staticPos(float x, float y) const;

	sf::Vector2f lastMousePos;
	sf::Vector2i chunkTl;
	sf::Sprite grassSprite;

	sf::RectangleShape preview;
	sf::Sprite shop;
	sf::Text resourcesText;
	
	GameInfo& info;
	Textures& textures;
	sf::Font font;

	sf::Sprite counter;
	sf::RectangleShape progress;
	sf::Sprite resourcesIcon[4];
};