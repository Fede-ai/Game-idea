#pragma once
#include <SFML/Graphics.hpp>

class State {
public:
	virtual int update(std::vector<sf::Event> events, float dTime) = 0;
	virtual void draw() = 0;

protected:
	State(sf::RenderWindow& inWindow) : window(inWindow) {}

	sf::RenderWindow& window;
};

class IngameState {
public:
	virtual int update(std::vector<sf::Event> events, float dTime) = 0;
	virtual void draw() = 0;

protected:
	IngameState(sf::RenderWindow& inWindow) : window(inWindow) {}

	sf::RenderWindow& window;
};