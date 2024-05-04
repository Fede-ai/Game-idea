#pragma once
#include "consts.hpp"
#include "state.hpp"

class GameState : public State
{
public:
	GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo, Settings inSettings);

private:
	int update(std::vector<sf::Event> events, float dTime);
	void draw();

	sf::RenderWindow& window;
	GameInfo& gameInfo;
	const Settings settings;
	sf::Font font;

	sf::Vector2f lastMousePos;

	sf::Texture grassTexture;
	sf::Sprite grassSprite;
	sf::Texture resourcesBgTexture;
	sf::Sprite resourcesBgSprite;

	sf::Texture resourcesTexture[6];
	sf::Sprite resourcesSprite[6];
	sf::Text resourcesText;
};

