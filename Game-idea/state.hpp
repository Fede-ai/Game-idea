#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

class State {
public:
	virtual ~State() {};
	virtual int update(std::vector<sf::Event> events, float dTime) = 0;
	virtual void draw() = 0;

protected:
	State(sf::RenderWindow& inWindow) : window(inWindow) {}

	sf::RenderWindow& window;
};

class IngameState {
public:
	virtual ~IngameState() {};
	virtual int update(std::vector<sf::Event> events, float dTime) = 0;
	virtual void draw() = 0;

protected:
	IngameState(sf::RenderWindow& inWindow) : window(inWindow) {}

	sf::RenderWindow& window;
};
