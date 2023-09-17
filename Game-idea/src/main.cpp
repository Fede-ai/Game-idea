#include "consts.h"
#include <iostream>

int main()
{
	bool isFullscreen = false;
	bool canFullscreen = false;
	sf::RenderWindow window(sf::VideoMode(Consts::windowSize.x, Consts::windowSize.y), Consts::gameName, sf::Style::Default);
	window.setView(sf::View(sf::Vector2f(Consts::viewSize.x/2, Consts::viewSize.y/2), Consts::viewSize));
	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)) 
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				//handle fullscreen
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
				{
					if (canFullscreen)
					{
						sf::View view = window.getView();
						if (isFullscreen)
							window.create(sf::VideoMode(Consts::windowSize.x, Consts::windowSize.y), Consts::gameName, sf::Style::Default);
						else
							window.create(sf::VideoMode::getDesktopMode(), Consts::gameName, sf::Style::Fullscreen);
						window.setFramerateLimit(60);
						window.setView(view);

						isFullscreen = !isFullscreen;
						canFullscreen = false;
					}
				}
				else
				{
					canFullscreen = true;
				}
			}
		}



		window.display();
	}

	return 0;
}