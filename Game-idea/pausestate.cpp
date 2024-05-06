#include "pausestate.hpp"
#include "consts.hpp"

PauseState::PauseState(sf::RenderWindow& inWindow)
	: 
	IngameState(inWindow)
{
	menuTexture.loadFromFile("textures/pause.png");
	menuSprite.setTexture(menuTexture);
	menuSprite.setScale(sf::Vector2f(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE));
	menuSprite.setOrigin(menuSprite.getLocalBounds().width / 2, menuSprite.getLocalBounds().height / 2);
}

int PauseState::update(std::vector<sf::Event> events, float dTime)
{
	return 0;
}

void PauseState::draw()
{
	window.draw(menuSprite);

}
