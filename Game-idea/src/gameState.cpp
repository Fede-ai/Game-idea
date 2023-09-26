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
	grassTexture.loadFromFile("texture/grass.png");
	grassSprite.setTexture(grassTexture);
	grassSprite.setScale(Consts::cellSize, Consts::cellSize);
	
	shop.setTexture(textures.shop);
	shop.setScale(Consts::pixelSize, Consts::pixelSize);

	woodSprite.setTexture(textures.wood);
	woodSprite.setScale(Consts::pixelSize, Consts::pixelSize);
	stoneSprite.setTexture(textures.stone);
	stoneSprite.setScale(Consts::pixelSize, Consts::pixelSize);
	goldSprite.setTexture(textures.gold);
	goldSprite.setScale(Consts::pixelSize, Consts::pixelSize);
	gemSprite.setTexture(textures.gem);
	gemSprite.setScale(Consts::pixelSize, Consts::pixelSize);
	
	window.setView(info.gameView);
	lastMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window)) - window.getView().getCenter();
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
				if (shop.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				{
					if (info.typeBuilding == -1)
						status = 1;
				}
				else if (info.typeBuilding != -1 && canPosition)
				{
					info.buildings.push_back(Building(textures, info.typeBuilding, preview.getPosition()));	
					if (info.typeBuilding == 0)
						setWallsTextures();
					info.typeBuilding = -1;
				}
			}
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::B && info.typeBuilding == -1)
			{
				status = 1;
			}
			else if (event.key.code == sf::Keyboard::V && info.typeBuilding == -1)
			{
				info.typeBuilding = info.lastBuilding;
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
	if (info.typeBuilding != -1)
	{
		preview.setSize(sf::Vector2f(Building::size[info.typeBuilding].x * Consts::cellSize, Building::size[info.typeBuilding].y * Consts::cellSize));
		previewHitbox.width = Building::size[info.typeBuilding].x;
		previewHitbox.height = Building::size[info.typeBuilding].y;

		sf::Vector2f previewPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		previewPos.x = (std::floor(previewPos.x / Consts::cellSize) - Building::size[info.typeBuilding].x / 2) * Consts::cellSize;
		previewPos.y = (std::floor(previewPos.y / Consts::cellSize) - Building::size[info.typeBuilding].y / 2) * Consts::cellSize;
		preview.setPosition(previewPos);		
		previewHitbox.left = previewPos.x / Consts::cellSize;
		previewHitbox.top = previewPos.y / Consts::cellSize;
	
		if (isSpaceEmpty(previewHitbox))
		{
			preview.setFillColor(sf::Color(20, 110, 20, 150));
			canPosition = true;	
		}
		else
		{
			preview.setFillColor(sf::Color(180, 30, 30, 150));
			canPosition = false;
		}
	}
	info.gameView = window.getView();

	//load new chunks
	int xChunk = std::floor(window.getView().getCenter().x / Consts::chunkSize.x - 0.5);
	int yChunk = std::floor(window.getView().getCenter().y / Consts::chunkSize.y - 0.5);
	bool newTl = true, newTr = true, newBl = true, newBr = true;
	for (auto chunk : loadedChucks)
	{
		if (chunk == sf::Vector2i(xChunk, yChunk))
			newTl = false;
		if (chunk == sf::Vector2i(xChunk + 1, yChunk))
			newTr = false;
		if (chunk == sf::Vector2i(xChunk, yChunk + 1))
			newBl = false;
		if (chunk == sf::Vector2i(xChunk + 1, yChunk + 1))
			newBr = false;
	}
	if (newTl)
	{
		loadedChucks.push_back(sf::Vector2i(xChunk, yChunk));
		spawnResouces(xChunk, yChunk);
	}
	if (newTr)
	{
		loadedChucks.push_back(sf::Vector2i(xChunk + 1, yChunk));
		spawnResouces(xChunk+1, yChunk);
	}	
	if (newBl)
	{
		loadedChucks.push_back(sf::Vector2i(xChunk, yChunk + 1));
		spawnResouces(xChunk, yChunk+1);
	}
	if (newBr)
	{
		loadedChucks.push_back(sf::Vector2i(xChunk + 1, yChunk + 1));
		spawnResouces(xChunk+1, yChunk+1);
	}

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
	int min = 10000, max = -10000;
	for (auto building : info.buildings)
	{
		min = std::min(min, building.pos.y);
		max = std::max(max, building.pos.y);
	}
	for (auto wood : info.wood)
	{
		min = std::min(min, wood.y);
		max = std::max(max, wood.y);
	}
	for (auto stone : info.stone)
	{
		min = std::min(min, stone.y);
		max = std::max(max, stone.y);
	}
	for (auto gold : info.gold)
	{
		min = std::min(min, gold.y);
		max = std::max(max, gold.y);
	}
	for (auto gem : info.gem)
	{
		min = std::min(min, gem.y);
		max = std::max(max, gem.y);
	}

	//draw all structures/resources in order
	for (int y = min; y < max; y++)
	{
		for (auto building : info.buildings)
		{
			if (building.pos.y == y)
				building.draw(window);
		}
		for (auto wood : info.wood)
		{
			if (wood.y == y)
			{
				woodSprite.setPosition(wood.x * Consts::cellSize, wood.y * Consts::cellSize);
				window.draw(woodSprite);
			}
		}
		for (auto stone : info.stone)
		{
			if (stone.y == y)
			{
				stoneSprite.setPosition(stone.x * Consts::cellSize, stone.y * Consts::cellSize);
				window.draw(stoneSprite);
			}
		}
		for (auto gold : info.gold)
		{
			if (gold.y == y)
			{
				goldSprite.setPosition(gold.x * Consts::cellSize, gold.y * Consts::cellSize);
				window.draw(goldSprite);
			}
		}
		for (auto gem : info.gem)
		{
			if (gem.y == y)
			{
				gemSprite.setPosition(gem.x * Consts::cellSize, gem.y * Consts::cellSize);
				window.draw(gemSprite);
			}
		}
	}
	
	if (info.typeBuilding != -1)
		window.draw(preview);
	
	shop.setPosition(window.getView().getCenter() + sf::Vector2f(1920/2-210, 1080/2-210));
	window.draw(shop);
}

void GameState::setWallsTextures()
{
	for (auto& building : info.buildings)
	{
		if (building.type == 0)
		{
			bool top = false, bot = false, left = false, right = false;
			for (auto other : info.buildings)
			{
				if (other.type == 0 && !(building.pos.x == other.pos.x && building.pos.y == other.pos.y))
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

bool GameState::isSpaceEmpty(sf::IntRect space)
{
	for (auto building : info.buildings)
	{
		if (space.intersects(building.hitbox))
			return false;
	}
	for (auto wood : info.wood)
	{
		sf::Vector2i woodSize = sf::Vector2i(woodSprite.getGlobalBounds().width / Consts::cellSize, woodSprite.getGlobalBounds().height / Consts::cellSize);
		sf::IntRect hitbox(wood, woodSize);
		if (space.intersects(hitbox))
			return false;
	}
	for (auto stone : info.stone)
	{
		sf::Vector2i stoneSize = sf::Vector2i(stoneSprite.getGlobalBounds().width / Consts::cellSize, stoneSprite.getGlobalBounds().height / Consts::cellSize);
		sf::IntRect hitbox(stone, stoneSize);
		if (space.intersects(hitbox))
			return false;
	}
	for (auto gold : info.gold)
	{
		sf::Vector2i goldSize = sf::Vector2i(goldSprite.getGlobalBounds().width / Consts::cellSize, goldSprite.getGlobalBounds().height / Consts::cellSize);
		sf::IntRect hitbox(gold, goldSize);
		if (space.intersects(hitbox))
			return false;
	}
	for (auto gem : info.gem)
	{
		sf::Vector2i gemSize = sf::Vector2i(gemSprite.getGlobalBounds().width / Consts::cellSize, gemSprite.getGlobalBounds().height / Consts::cellSize);
		sf::IntRect hitbox(gem, gemSize);
		if (space.intersects(hitbox))
			return false;
	}
	return true;
}

void GameState::spawnResouces(int x, int y)
{
	srand(x * 2 + y + 3);
	auto random = [](int max) {
		return (rand() % max);
	};

	int xOff = x*Consts::chunkSize.x / Consts::cellSize;
	int yOff = y*Consts::chunkSize.y / Consts::cellSize;

	for (int i = 0; i < 12; i++)
	{
		info.wood.push_back(sf::Vector2i(xOff+random(240), yOff+random(135)));
	}
	for (int i = 0; i < 9; i++)
	{
		info.stone.push_back(sf::Vector2i(xOff+random(240), yOff+random(135)));
	}
	for (int i = 0; i < 6; i++)
	{
		info.gold.push_back(sf::Vector2i(xOff+random(240), yOff+random(135)));
	}
	for (int i = 0; i < 3; i++)
	{
		info.gem.push_back(sf::Vector2i(xOff+random(240), yOff+random(135)));
	}
}
