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
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window)) - window.getView().getCenter();
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		sf::View view(window.getView());
		view.move(lastMousePos - mousePos);
		float x = std::min(std::max(Consts::viewSize.x/2.f, view.getCenter().x), Consts::fieldSize.x - Consts::viewSize.x / 2.f);
		float y = std::min(std::max(Consts::viewSize.y/2.f, view.getCenter().y), Consts::fieldSize.y - Consts::viewSize.y / 2.f);
		view.setCenter(x, y);
		window.setView(view);
	}
	lastMousePos = mousePos;
	
	if (info.typeBuilding != -1)
	{
		preview.setSize(sf::Vector2f(Building::size[info.typeBuilding].x * Consts::cellSize, Building::size[info.typeBuilding].y * Consts::cellSize));
		sf::Vector2f previewPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		previewPos.x = (int(previewPos.x / Consts::cellSize) - Building::size[info.typeBuilding].x / 2) * Consts::cellSize;
		previewPos.x = std::min(std::max(0.f, previewPos.x), Consts::fieldSize.x - preview.getSize().x);
		previewPos.y = (int(previewPos.y / Consts::cellSize) - Building::size[info.typeBuilding].y / 2) * Consts::cellSize;
		previewPos.y = std::min(std::max(0.f, previewPos.y), Consts::fieldSize.y - preview.getSize().y);
		preview.setPosition(previewPos);
	
		preview.setFillColor(sf::Color(20, 110, 20, 150));
		canPosition = true;	
		for (auto building : info.buildings)
		{			
			if (preview.getGlobalBounds().intersects(building.hitbox.getGlobalBounds()))
			{
				preview.setFillColor(sf::Color(180, 30, 30, 150));
				canPosition = false;
				break;
			}				
		}
	}
	info.gameView = window.getView();

	return status;
}

void GameState::draw()
{
	//draw grass
	for (int x = 0; x < Consts::fieldSize.x/(Consts::cellSize * 10)+1; x++)
	{
		for (int y = 0; y < Consts::fieldSize.y/(Consts::cellSize * 10)+1; y++)
		{
			grassSprite.setPosition(x * Consts::cellSize * 10, y * Consts::cellSize * 10);
			window.draw(grassSprite);
		}
	}
	
	for (int y = 0; y < 180; y++)
	{
		for (auto building : info.buildings)
		{
			if (building.pos.y == y)
				building.draw(window);
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