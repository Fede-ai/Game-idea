#pragma once
#include <SFML/Graphics.hpp>

class State {
public:
	int processFrame(std::vector<sf::Event> events, float dTime) {
		int whatHappened = update(events, dTime);
		draw();
		window.display();

		return whatHappened;
	}
	// !! EXPENSIVE !!
	sf::Texture screenshot() {
		draw();
		sf::Texture texture;
		texture.create(window.getSize().x, window.getSize().y);
		texture.update(window);
		return texture;
	}

protected:
	State(sf::RenderWindow& inWindow) : window(inWindow) {}

	virtual int update(std::vector<sf::Event> events, float dTime) = 0;
	virtual void draw() = 0;

	sf::RenderWindow& window;
};