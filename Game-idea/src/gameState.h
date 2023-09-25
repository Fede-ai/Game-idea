#pragma once
#include "state.h"
#include "consts.h"
#include "building.h"

struct GameInfo
{
	GameInfo();
	int typeBuilding = -1;
	int lastBuilding = -1;
	std::vector<Building> buildings;
	sf::View gameView = sf::View(sf::Vector2f(Consts::fieldSize.x/2, Consts::fieldSize.y/2), Consts::viewSize);

	std::vector<sf::Vector2i> wood;
	std::vector<sf::Vector2i> stone;
	std::vector<sf::Vector2i> gold;
	std::vector<sf::Vector2i> gem;

	int nWood = 0;
	int nStone = 0;
	int nGold = 0;
	int nGem = 0;
	int population = 0;

	int day = 0;
	int time = 0;
};

class GameState : public State
{
public:
	GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo, Textures& inTextures);
	void handleEvents();
	int update();
	void draw();

private:
	void setWallsTextures();
	bool isSpaceEmpty(sf::IntRect space);
	sf::Vector2f lastMousePos;
	
	sf::Texture grassTexture;
	sf::Sprite grassSprite;
	
	sf::RectangleShape preview;
	sf::IntRect previewHitbox;
	bool canPosition = false;
	sf::Sprite shop;
	
	GameInfo& info;
	Textures& textures;

	sf::Sprite woodSprite;
	sf::Sprite stoneSprite;
	sf::Sprite goldSprite;
	sf::Sprite gemSprite;
};