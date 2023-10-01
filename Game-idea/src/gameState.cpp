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
	shop.setScale(Consts::pixelSize*2, Consts::pixelSize*2);

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
	
	if (info.wood.size() == 0)
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

	std::cout << info.wood.size() << "\n";

	//load and unload new chunks
	int xChunk = std::floor(window.getView().getCenter().x / Consts::chunkSize.x - 0.5);
	int yChunk = std::floor(window.getView().getCenter().y / Consts::chunkSize.y - 0.5);
	if (chunkTl != sf::Vector2i(xChunk, yChunk))
	{
		clearResources();
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
	int min = info.wood[0].y, max = info.wood[0].y;
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
	
	shop.setPosition(window.getView().getCenter() + sf::Vector2f(1920/2-250, 1080/2-250));
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
bool GameState::canSpawnResource(sf::IntRect space)
{
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

	for (int i = 0; i < 10; i++)
	{
		sf::Vector2f size(woodSprite.getGlobalBounds().getSize());
		sf::Vector2i cellSize(size.x/Consts::cellSize+6, size.y / Consts::cellSize+6);
		sf::IntRect hitbox(sf::Vector2i(xOff + random(xMax), yOff + random(yMax)), cellSize);

		if (canSpawnResource(hitbox))
			info.wood.push_back(sf::Vector2i(hitbox.left+3, hitbox.top+3));
		else
			i--;
	}
	for (int i = 0; i < 7; i++)
	{
		sf::Vector2f size(stoneSprite.getGlobalBounds().getSize());
		sf::Vector2i cellSize(size.x / Consts::cellSize+6, size.y / Consts::cellSize+6);
		sf::IntRect hitbox(sf::Vector2i(xOff + random(xMax), yOff + random(yMax)), cellSize);

		if (canSpawnResource(hitbox))
			info.stone.push_back(sf::Vector2i(hitbox.left+3, hitbox.top+3));
		else
			i--;
	}
	for (int i = 0; i < 5; i++)
	{
		sf::Vector2f size(goldSprite.getGlobalBounds().getSize());
		sf::Vector2i cellSize(size.x / Consts::cellSize+6, size.y / Consts::cellSize+6);
		sf::IntRect hitbox(sf::Vector2i(xOff + random(xMax), yOff + random(yMax)), cellSize);

		if (canSpawnResource(hitbox))
			info.gold.push_back(sf::Vector2i(hitbox.left+3, hitbox.top+3));
		else
			i--;
	}
	for (int i = 0; i < 2; i++)
	{
		sf::Vector2f size(gemSprite.getGlobalBounds().getSize());
		sf::Vector2i cellSize(size.x / Consts::cellSize+6, size.y / Consts::cellSize+6);
		sf::IntRect hitbox(sf::Vector2i(xOff + random(xMax), yOff + random(yMax)), cellSize);

		if (canSpawnResource(hitbox))
			info.gem.push_back(sf::Vector2i(hitbox.left+3, hitbox.top+3));
		else
			i--;
	}
}
void GameState::clearResources()
{
	/*sf::IntRect chunk(x * Consts::chunkSize.x / Consts::cellSize, y * Consts::chunkSize.y / Consts::cellSize,
		Consts::chunkSize.x / Consts::cellSize, Consts::chunkSize.y / Consts::cellSize);

	for (int i = 0; i < info.wood.size(); i++)
	{
		if (chunk.contains(info.wood[i]))
		{
			info.wood.erase(info.wood.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < info.stone.size(); i++)
	{
		if (chunk.contains(info.stone[i]))
		{
			info.stone.erase(info.stone.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < info.gold.size(); i++)
	{
		if (chunk.contains(info.gold[i]))
		{
			info.gold.erase(info.gold.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < info.gem.size(); i++)
	{
		if (chunk.contains(info.gem[i]))
		{
			info.gem.erase(info.gem.begin() + i);
			i--;
		}
	}*/
	info.wood.clear();
	info.stone.clear();
	info.gold.clear();
	info.gem.clear();
}
