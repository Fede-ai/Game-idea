#include "shopstate.hpp"

ShopState::ShopState(sf::RenderWindow& inWindow)
	: State(inWindow)
{
	menuTexture.loadFromFile("textures/shop.png");
	menuSprite.setTexture(menuTexture);
	menuSprite.setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);
	menuSprite.setPosition(-Consts::VIEW_SIZE_X / 2, -Consts::VIEW_SIZE_Y / 2);
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
