#include "gamestate.hpp"

int main() {
	Settings settings;
	GameInfo gameInfo;

	sf::ContextSettings OpenGLSettings;
	OpenGLSettings.antialiasingLevel = 8;
	auto width = sf::VideoMode::getDesktopMode().width;
	sf::RenderWindow window(sf::VideoMode(int(width * 2 / 3.f), int(width * 3 / 8.f)), Consts::GAME_NAME, sf::Style::Default, OpenGLSettings);
	window.setView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080)));
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);
	
	State* state = new GameState(window, gameInfo, settings);

	while (window.isOpen()) {
		sf::Event e;
		std::vector<sf::Event> events;
		while (window.pollEvent(e)) {
			//toggle fullscreen
			if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter && e.key.alt) {
				sf::View view = window.getView();
				if (window.getSize().x == sf::VideoMode::getFullscreenModes()[0].width)
					window.create(sf::VideoMode(int(width * 2 / 3.f), int(width * 3 / 8.f)), Consts::GAME_NAME, sf::Style::Default, OpenGLSettings);
				else
					window.create(sf::VideoMode::getDesktopMode(), Consts::GAME_NAME, sf::Style::Fullscreen, OpenGLSettings);
				window.setView(view);
				window.setVerticalSyncEnabled(true);
				window.setKeyRepeatEnabled(false);
			}

			events.push_back(e);
		}

		int whatHappened = state->processFrame(events);
	}

	return 0;
}