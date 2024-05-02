#pragma once
#include <SFML/Graphics.hpp>
#include "gameinfo.hpp"
#include "settings.hpp"

class State {
public:
	inline int processFrame(std::vector<sf::Event> events, float dTime) {
		int whatHappened = update(events, dTime);
		draw();
		return whatHappened;
	}

private:
	virtual int update(std::vector<sf::Event> events, float dTime) = 0;
	virtual void draw() = 0;
};