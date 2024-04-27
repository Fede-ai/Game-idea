#pragma once
#include "state.hpp"

class GameState : public State
{
public:
	GameState(sf::RenderWindow* inWindow);

private:
	int update(std::vector<sf::Event> events);
	void draw();

	sf::RenderWindow* window;

	sf::Texture grassTexture;
	sf::Sprite grassSprite;

	sf::Vector2f lastMousePos;
};

