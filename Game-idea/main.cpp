#include "gamestate.hpp"
#include "homestate.hpp"
#include <chrono>

int main() {
	Settings settings;
	GameInfo gameInfo;

	sf::Image cursorImage, scaledCursor;
	cursorImage.loadFromFile("textures/cursor.png");
	scaledCursor.create(cursorImage.getSize().x * Consts::PIXEL_SIZE, cursorImage.getSize().y * Consts::PIXEL_SIZE);
	for (int x = 0; x < cursorImage.getSize().x; x++) {
		for (int y = 0; y < cursorImage.getSize().y; y++) {
			for (int a = 0 ; a < Consts::PIXEL_SIZE; a++) {
				for (int b = 0; b < Consts::PIXEL_SIZE; b++) {
					scaledCursor.setPixel(x * Consts::PIXEL_SIZE + a, y * Consts::PIXEL_SIZE + b, cursorImage.getPixel(x, y));
				}
			}
		}
	}
	sf::Cursor cursor;
	cursor.loadFromPixels(scaledCursor.getPixelsPtr(), scaledCursor.getSize(), sf::Vector2u(0, 0));

	sf::ContextSettings OpenGLSettings;
	OpenGLSettings.antialiasingLevel = 8;
	auto width = sf::VideoMode::getDesktopMode().width;
	sf::RenderWindow window(sf::VideoMode(int(width * 2 / 3.f), int(width * 3 / 8.f)), Consts::GAME_NAME, sf::Style::Default, OpenGLSettings);
	window.setView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080)));
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);
	window.setMouseCursor(cursor);
	
	auto lastTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	State* state = new HomeState(window);

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
				window.setMouseCursor(cursor);
			}

			events.push_back(e);
		}

		auto time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		int whatHappened = state->processFrame(events, int(std::max(time - lastTime, long long(1))) / 1000.f);
		lastTime = time;

		if (whatHappened == 1) {
			delete state;
			state = new GameState(window, gameInfo, settings);
		}
	}

	return 0;
}