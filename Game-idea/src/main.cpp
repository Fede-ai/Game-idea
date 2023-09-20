#include "consts.h"
#include "gameState.h"

int main()
{
	bool isFullscreen = false;
	bool canFullscreen = false;
	sf::RenderWindow window(sf::VideoMode(Consts::windowSize.x, Consts::windowSize.y), Consts::gameName, sf::Style::Default);
	window.setView(sf::View(sf::Vector2f(0, 0), Consts::viewSize));
	window.setFramerateLimit(60);

	GameInfo gameInfo;
	State* state = new GameState(window, gameInfo);

	while (window.isOpen())
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

		state->handleEvents();
		int status = state->update();

		window.clear(sf::Color::Black);
		state->draw();
		window.display();
	}

	return 0;
}