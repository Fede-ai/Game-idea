#include "gameState.h"

GameState::GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo)
	:
	State(inWindow),
	info(inGameInfo)
{
	grassTexture.loadFromFile("texture/grass.png");
	grassSprite.setTexture(grassTexture);
	grassSprite.setScale(6, 6);

	shop.setSize(sf::Vector2f(150, 150));

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
					status = 1;
				}
				else if (info.typeBuilding != -1 )
				{
					info.buildings.push_back(Building(info.typeBuilding, preview.getPosition()));
					info.typeBuilding = -1;
				}
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
		float x = std::min(std::max(-1920.f, view.getCenter().x), 1920.f);
		float y = std::min(std::max(-1080.f, view.getCenter().y), 1080.f);
		view.setCenter(x, y);
		window.setView(view);
	}
	lastMousePos = mousePos;

	if (info.typeBuilding != -1)
	{
		preview.setSize(sf::Vector2f(Building::size[info.typeBuilding].x * 6, Building::size[info.typeBuilding].y * 6));
		sf::Vector2f previewPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		previewPos.x = (round(previewPos.x / 6) - Building::size[info.typeBuilding].x / 2) * 6;
		previewPos.y = (round(previewPos.y / 6) - Building::size[info.typeBuilding].y / 2) * 6;
		preview.setPosition(previewPos);
	}
	info.gameView = window.getView();

	return status;
}

void GameState::draw()
{
	//draw grass
	for (int x = 0; x < 32*3; x++)
	{
		for (int y = 0; y < 18*3; y++)
		{
			grassSprite.setPosition(x * 60 - 1920*1.5, y * 60 - 1080*1.5);
			window.draw(grassSprite);
		}
	}

	if (info.typeBuilding != -1)
		window.draw(preview);

	for (auto building : info.buildings)
		building.draw(window);

	shop.setPosition(window.getView().getCenter() + sf::Vector2f(1920/2-200, 1080/2-200));
	window.draw(shop);
}
