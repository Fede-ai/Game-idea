#pragma once
#include <SFML/Graphics.hpp>
#include "gameinfo.hpp"
#include "settings.hpp"

class State {
public:
	inline int processFrame(std::vector<sf::Event> events) {
		int whatHappened = update(events);
		draw();
		return whatHappened;
	}

private:
	virtual int update(std::vector<sf::Event> events) = 0;
	virtual void draw() = 0;
};