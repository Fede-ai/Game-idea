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

	sf::RenderWindow& window;
	IngameState(sf::RenderWindow& inWindow) : window(inWindow) {}
protected:

};
