#include "gameState.h"
#include <iostream>

GameInfo::GameInfo()
{
}

GameState::GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo, Textures& inTextures)
	:
	State(inWindow),
	info(inGameInfo),
	textures(inTextures)
{
	font.loadFromFile("font.ttf"); 
	resourcesText.setCharacterSize(60);
	resourcesText.setFont(font);

	grassSprite.setTexture(textures.grass);
	grassSprite.setScale(Consts::cellSize, Consts::cellSize);
	
	shop.setTexture(textures.shop);
	shop.setScale(Consts::pixelSize*1.5, Consts::pixelSize*1.5);
	
	window.setView(info.gameView);
	lastMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window)) - window.getView().getCenter();
	
	if (info.resources.size() == 0)
	{
		spawnResources(-1, -1);
		spawnResources(-1, 0);
		spawnResources(0, -1);
		spawnResources(0, 0);
	}
	int xChunk = std::floor(window.getView().getCenter().x / Consts::chunkSize.x - 0.5);
	int yChunk = std::floor(window.getView().getCenter().y / Consts::chunkSize.y - 0.5);
	chunkTl = sf::Vector2i(xChunk, yChunk);
}

void GameState::handleEvents()
{	
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//open shop
				if (shop.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				{
					if (info.typeBuilding == Building::types::none)
						status = 1;
				}
				//place building
				else if (info.typeBuilding != Building::types::none)
				{
					Building building(textures, info.typeBuilding, preview.getPosition());
					if (canPlaceBuilding(building))
					{
						info.buildings.push_back(building);	
						if (info.typeBuilding == Building::types::wall)
							updateWallsTextures();
						info.typeBuilding = Building::types::none;
					}
				}
			}
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::B && info.typeBuilding == Building::types::none)
			{
				status = 1;
			}
			else if (event.key.code == sf::Keyboard::V && info.typeBuilding == Building::types::none)
			{
				info.typeBuilding = info.lastBuilding;
			}
			else if (event.key.code == sf::Keyboard::Escape && info.typeBuilding != Building::types::none)
			{
				info.typeBuilding = Building::types::none;
			}
		}
	}
}
int GameState::update()
{
	//move view
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window)) - window.getView().getCenter();
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		sf::View view(window.getView());
		view.move(lastMousePos - mousePos);
		window.setView(view);
	}
	lastMousePos = mousePos;
	
	//move preview and control if the space is empty
	if (info.typeBuilding != Building::types::none)
	{
		//set size and position of preview square
		preview.setSize(sf::Vector2f(Building::size[info.typeBuilding].x * Consts::cellSize, Building::size[info.typeBuilding].y * Consts::cellSize));
		sf::Vector2f previewPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		previewPos.x = (std::floor(previewPos.x / Consts::cellSize) - Building::size[info.typeBuilding].x / 2) * Consts::cellSize;
		previewPos.y = (std::floor(previewPos.y / Consts::cellSize) - Building::size[info.typeBuilding].y / 2) * Consts::cellSize;
		preview.setPosition(previewPos);

		Building previewHitbox(textures, info.typeBuilding, previewPos);
		if (canPlaceBuilding(previewHitbox))
			preview.setFillColor(sf::Color(20, 110, 20, 150));
		else
			preview.setFillColor(sf::Color(180, 30, 30, 150));
	}
	info.gameView = window.getView();

	//load and unload new chunks
	int xChunk = std::floor(window.getView().getCenter().x / Consts::chunkSize.x - 0.5);
	int yChunk = std::floor(window.getView().getCenter().y / Consts::chunkSize.y - 0.5);
	if (chunkTl != sf::Vector2i(xChunk, yChunk))
	{
		info.resources.clear();
		spawnResources(xChunk, yChunk);
		spawnResources(xChunk, yChunk+1);
		spawnResources(xChunk+1, yChunk);
		spawnResources(xChunk+1, yChunk+1);
	}
	chunkTl = sf::Vector2i(xChunk, yChunk);

	return status;
}
void GameState::draw()
{
	//draw grass
	int xChunk = std::floor(window.getView().getCenter().x / Consts::chunkSize.x - 0.5);
	int yChunk = std::floor(window.getView().getCenter().y / Consts::chunkSize.y - 0.5);
	for (int x = 0; x < Consts::chunkSize.x/(Consts::cellSize * 8); x++)
	{
		for (int y = 0; y < Consts::chunkSize.y/(Consts::cellSize * 8); y++)
		{
			grassSprite.setPosition(xChunk * Consts::chunkSize.x + x * Consts::cellSize * 8, yChunk * Consts::chunkSize.y + y * Consts::cellSize * 8);
			window.draw(grassSprite);
			grassSprite.setPosition((xChunk+1) * Consts::chunkSize.x + x * Consts::cellSize * 8, yChunk * Consts::chunkSize.y + y * Consts::cellSize * 8);
			window.draw(grassSprite);
			grassSprite.setPosition(xChunk * Consts::chunkSize.x + x * Consts::cellSize * 8, (yChunk+1) * Consts::chunkSize.y + y * Consts::cellSize * 8);
			window.draw(grassSprite);
			grassSprite.setPosition((xChunk+1) * Consts::chunkSize.x + x * Consts::cellSize * 8, (yChunk+1) * Consts::chunkSize.y + y * Consts::cellSize * 8);
			window.draw(grassSprite);
		}
	}

	//calculate the highest and lowest structure
	int min = info.resources[0].pos.y, max = info.resources[0].pos.y;
	for (auto building : info.buildings)
	{
		min = std::min(min, building.pos.y);
		max = std::max(max, building.pos.y);
	}
	for (auto resource : info.resources)
	{
		min = std::min(min, resource.pos.y);
		max = std::max(max, resource.pos.y);
	}

	//draw all structures/resources in order
	for (int y = min; y < max; y++)
	{
		for (auto building : info.buildings)
		{
			if (building.pos.y == y)
				building.draw(window);
		}
		for (auto resource : info.resources)
		{
			if (resource.pos.y == y)
			{
				resource.draw(window);
			}
		}
	}
	
	if (info.typeBuilding != Building::types::none)
		window.draw(preview);
	
	shop.setPosition(window.getView().getCenter() + sf::Vector2f(1920/2-250, 1080/2-250));
	window.draw(shop);

	resourcesText.setString("W: " + std::to_string(info.nWood) + ", S: " + std::to_string(info.nStone) +
		", GO: " + std::to_string(info.nGold) + ", GE: " + std::to_string(info.nGem));
	resourcesText.setPosition(window.getView().getCenter() + sf::Vector2f(1920/2 - resourcesText.getGlobalBounds().width - 20, -1080 / 2));
	window.draw(resourcesText);
}

void GameState::updateWallsTextures()
{
	for (auto& building : info.buildings)
	{
		if (building.type == Building::types::wall)
		{
			bool top = false, bot = false, left = false, right = false;
			for (auto other : info.buildings)
			{
				if (other.type == Building::types::wall && !(building.pos.x == other.pos.x && building.pos.y == other.pos.y))
				{
					if (building.pos.x == other.pos.x)
					{
						if (other.pos.y == building.pos.y + 1)
						{
							bot = true;
						}
						else if (other.pos.y == building.pos.y - 1)
						{
							top = true;
						}
					}
					else if (building.pos.y == other.pos.y)
					{
						if (other.pos.x == building.pos.x + 1)
						{
							right = true;
						}
						else if (other.pos.x == building.pos.x - 1)
						{
							left = true;
						}
					}
				}
			}
	
			int n = top * 8 + bot * 4 + right * 2 + left;
			building.setTextureNum(textures, n);
		}
	}
}
bool GameState::canPlaceBuilding(Building building)
{
	for (auto other : info.buildings)
	{
		if (building.hitbox.intersects(other.hitbox))
			return false;
	}
	for (auto resource : info.resources)
	{
		if (building.hitbox.intersects(resource.hitbox))
			return false;
	}

	if (building.type == Building::types::woodFarm)
	{
		for (auto resource : info.resources)
		{
			if (resource.type == Resource::types::wood && resource.isBuildingInRange(building))
				return true;
		}
		return false;
	}
	else if (building.type == Building::types::stoneFarm)
	{
		for (auto resource : info.resources)
		{
			if (resource.type == Resource::types::stone && resource.isBuildingInRange(building))
				return true;
		}
		return false;
	}
	else if (building.type == Building::types::goldFarm)
	{
		for (auto resource : info.resources)
		{
			if (resource.type == Resource::types::gold && resource.isBuildingInRange(building))
				return true;
		}
		return false;
	}
	else if (building.type == Building::types::gemFarm)
	{
		for (auto resource : info.resources)
		{
			if (resource.type == Resource::types::gem && resource.isBuildingInRange(building))
				return true;
		}
		return false;
	}
	return true;
}

void GameState::spawnResources(int x, int y)
{
	srand(info.seed + x * 2 + y + 3);
	auto random = [](int max) {
		return (rand() % max);
	};

	int xOff = x*Consts::chunkSize.x / Consts::cellSize;
	int yOff = y*Consts::chunkSize.y / Consts::cellSize;

	const int xMax = Consts::chunkSize.x / Consts::cellSize;
	const int yMax = Consts::chunkSize.y / Consts::cellSize;

	const int woodPC = 7;
	const int stonePC = 5;
	const int goldPC = 3;
	const int gemPC = 1;

	int type = Resource::types::wood;
	for (int i = 0; i < woodPC + stonePC + goldPC + gemPC; i++)
	{
		if (i == woodPC)
			type = Resource::types::stone;
		else if (i == woodPC + stonePC)
			type = Resource::types::gold;
		else if (i == woodPC + stonePC + goldPC)
			type = Resource::types::gem;

		sf::Vector2f pos((xOff + random(xMax)) * Consts::cellSize, (yOff + random(yMax)) * Consts::cellSize);
		Resource resource(textures, type, pos);
		if (canSpawnResource(resource))
			info.resources.push_back(resource);
		else
			i--;
	}
}
bool GameState::canSpawnResource(Resource resource)
{
	for (auto other : info.resources)
	{
		if (resource.spawnHitbox.intersects(other.hitbox))
			return false;
	}

	return true;
}