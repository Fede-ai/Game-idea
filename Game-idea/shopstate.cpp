#include "shopstate.hpp"

ShopState::ShopState(sf::RenderWindow& inWindow)
	: State(inWindow)
{
	menuTexture.loadFromFile("textures/shop.png");
	menuSprite.setTexture(menuTexture);
	menuSprite.setScale(CON::PIXEL_SIZE, CON::PIXEL_SIZE);
	menuSprite.setPosition(-CON::VIEW_WIDTH / 2, -CON::VIEW_HEIGHT / 2);
}

int ShopState::update(std::vector<sf::Event> events, float dTime)
{
	return 0;
}

void ShopState::draw()
{
	window.clear(sf::Color::Black);

	window.draw(menuSprite);
}
