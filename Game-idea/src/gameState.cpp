#include "gameState.h"

GameState::GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo)
	:
	State(inWindow),
	GameInfo(inGameInfo)
{
	grassTexture.loadFromFile("C:/Users/feder/Desktop/pixil-frame-0.png");
	grassSprite.setTexture(grassTexture);
	grassSprite.setScale(6, 6);

	sf::Vector2i pos = sf::Mouse::getPosition(window);
	float xProp = window.getView().getSize().x / window.getSize().x;
	float yProp = window.getView().getSize().x / window.getSize().x;
	lastMousePos = sf::Vector2f(pos.x * xProp, pos.y * yProp);
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
				if (isPositioning != -1)
				{
					buildings.push_back(Building(isPositioning, preview.getPosition()));
					isPositioning = -1;
				}
			}
		}
	}
}

int GameState::update()
{
	sf::Vector2i pos = sf::Mouse::getPosition(window);
	float xProp = window.getView().getSize().x / window.getSize().x;
	float yProp = window.getView().getSize().x / window.getSize().x;
	sf::Vector2f mousePos = sf::Vector2f(pos.x * xProp, pos.y * yProp);

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

	if (isPositioning != -1)
	{
		preview.setSize(sf::Vector2f(Building::size[isPositioning].x * 6, Building::size[isPositioning].y * 6));
		sf::Vector2f previewPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		previewPos.x = (round(previewPos.x / 6) - Building::size[isPositioning].x / 2) * 6;
		previewPos.y = (round(previewPos.y / 6) - Building::size[isPositioning].y / 2) * 6;
		preview.setPosition(previewPos);
	}

	return 0;
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

	if (isPositioning != -1)
	{
		window.draw(preview);
	}

	for (auto building : buildings)
		building.draw(window);
}
