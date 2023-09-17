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
	virtual int update() = 0;
	virtual void draw() = 0;

private:
	sf::RenderWindow& window;
};