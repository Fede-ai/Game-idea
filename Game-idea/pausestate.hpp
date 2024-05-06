#pragma once
#include "state.hpp"

class PauseState : public IngameState {
public:
	PauseState(sf::RenderWindow& inWindow);

	int update(std::vector<sf::Event> events, float dTime);
	void draw();
private:
	sf::Texture menuTexture;
	sf::Sprite menuSprite;
};