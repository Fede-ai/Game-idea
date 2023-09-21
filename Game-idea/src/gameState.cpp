#include "gameState.h"

GameState::GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo)
	:
	State(inWindow),
	info(inGameInfo)
{
	grassTexture.loadFromFile("texture/grass.png");
	grassSprite.setTexture(grassTexture);
	grassSprite.setScale(Consts::pixelSize, Consts::pixelSize);

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
				if (shop.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))) && info.typeBuilding == -1)
				{
					status = 1;
				}
				else if (info.typeBuilding != -1 && canPosition)
				{
					info.buildings.push_back(Building(info.typeBuilding, preview.getPosition()));
					info.typeBuilding = -1;
				}
			}
		}
		else if (event.type == sf::Event::KeyPressed )
		{
			if (event.key.code == sf::Keyboard::B && info.typeBuilding == -1)
			{
				status = 1;
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
		preview.setSize(sf::Vector2f(Building::size[info.typeBuilding].x * Consts::pixelSize, Building::size[info.typeBuilding].y * Consts::pixelSize));
		sf::Vector2f previewPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		previewPos.x = (int(previewPos.x / Consts::pixelSize) - Building::size[info.typeBuilding].x / 2) * Consts::pixelSize;
		previewPos.x = std::min(std::max(0.f, previewPos.x), Consts::fieldSize.x - preview.getSize().x);
		previewPos.y = (int(previewPos.y / Consts::pixelSize) - Building::size[info.typeBuilding].y / 2) * Consts::pixelSize;
		previewPos.y = std::min(std::max(0.f, previewPos.y), Consts::fieldSize.y - preview.getSize().y);
		preview.setPosition(previewPos);

		preview.setFillColor(sf::Color::Green);
		canPosition = true;	
		for (auto building : info.buildings)
		{			
			if (preview.getGlobalBounds().intersects(building.body.getGlobalBounds()))
			{
				preview.setFillColor(sf::Color::Red);
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
	for (int x = 0; x < Consts::fieldSize.x/(Consts::pixelSize * 10)+1; x++)
	{
		for (int y = 0; y < Consts::fieldSize.y/(Consts::pixelSize * 10)+1; y++)
		{
			grassSprite.setPosition(x * Consts::pixelSize * 10, y * Consts::pixelSize * 10);
			window.draw(grassSprite);
		}
	}

	for (auto building : info.buildings)
		building.draw(window);	

	if (info.typeBuilding != -1)
		window.draw(preview);

	shop.setPosition(window.getView().getCenter() + sf::Vector2f(1920/2-200, 1080/2-200));
	window.draw(shop);
}
