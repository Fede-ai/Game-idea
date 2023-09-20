#pragma once
#include <SFML/Graphics.hpp>

class State
{
public:
	State(sf::RenderWindow& inWindow)
		:
		window(inWindow)
	{
	}
	virtual void handleEvents() = 0;
	virtual int update() = 0;
	virtual void draw() = 0;

protected:
	sf::RenderWindow& window;
};