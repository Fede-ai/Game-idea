#include "shopState.h"
#include <iostream>

ShopState::ShopState(sf::RenderWindow& inWindow, GameInfo& inGameInfo)
	:
	State(inWindow),
	info(inGameInfo)
{
	sf::View view(sf::Vector2f(Consts::viewSize.x / 2, Consts::viewSize.y / 2), Consts::viewSize);
	window.setView(view);
	itemBg.setSize(sf::Vector2f((1920 - space * (itemPerRow + 1)) / itemPerRow, 500));
	//30
}

void ShopState::handleEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
		else if (event.type == sf::Event::MouseWheelScrolled)
		{
			sf::View view(window.getView());
			view.move(0, -event.mouseWheelScroll.delta * 64);
			float yMax = space + (itemBg.getSize().y + space) * int(nItems / itemPerRow + 1);
			view.setCenter(Consts::viewSize.x / 2, std::min(view.getCenter().y, yMax - Consts::viewSize.y / 2));
			view.setCenter(Consts::viewSize.x / 2, std::max(view.getCenter().y, Consts::viewSize.y / 2));
			window.setView(view);
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				for (int i = 0; i < nItems; i++)
				{
					int y = i / itemPerRow;
					int nLine = i - y * itemPerRow;
					itemBg.setPosition(space + (itemBg.getSize().x + space) * nLine, space + (itemBg.getSize().y + space) * y);
					if (itemBg.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					{
						info.typeBuilding = i;
						status = 2;
					}
				}
			}
		}
	}
}

int ShopState::update()
{
	
	return status;
}

void ShopState::draw()
{
	window.clear(sf::Color(sf::Color(80, 80, 80)));	
	for (int i = 0; i < nItems; i++)
	{
		int y = i / itemPerRow;
		int nLine = i - y * itemPerRow;
		itemBg.setPosition(space + (itemBg.getSize().x + space) * nLine, space + (itemBg.getSize().y + space) * y);
		window.draw(itemBg);
	}
}
