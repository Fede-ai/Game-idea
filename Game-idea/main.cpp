#include "gamestate.hpp"

int main() {
	Settings settings;
	GameInfo gameInfo;

	sf::ContextSettings OpenGLSettings;
	OpenGLSettings.antialiasingLevel = 8;
	//sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Game-idea", sf::Style::Fullscreen, OpenGLSettings);
	sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[1], "Game-idea", sf::Style::Default, OpenGLSettings);
	window.setView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080)));
	window.setVerticalSyncEnabled(true);
	
	State* state = new GameState(&window);

	while (window.isOpen()) {
		sf::Event e;
		std::vector<sf::Event> events;
		while (window.pollEvent(e))
			events.push_back(e);

		int whatHappened = state->processFrame(events);
	}

	return 0;
}