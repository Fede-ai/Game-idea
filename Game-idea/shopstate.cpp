#include "shopstate.hpp"

ShopState::ShopState(sf::RenderWindow& inWindow)
	: State(inWindow)
{
	int VIEW_X = CON::VIEW_SIZE_X / 2, VIEW_Y = CON::VIEW_SIZE_Y / 2;
	//menu
	menuTexture.loadFromFile("textures/shop.png");
	menuSprite.setTexture(menuTexture);
	menuSprite.setScale(CON::PIXEL_SIZE, CON::PIXEL_SIZE);
	menuSprite.setPosition(-VIEW_X, -VIEW_Y);

	//currencies
	woodTexture.loadFromFile("textures/currencies/wood.png");
	stoneTexture.loadFromFile("textures/currencies/stone.png");

	wood.setTexture(woodTexture);
	wood.setScale(sf::Vector2f(CON::PIXEL_SIZE, CON::PIXEL_SIZE));

	stone.setTexture(stoneTexture);
	stone.setScale(sf::Vector2f(CON::PIXEL_SIZE, CON::PIXEL_SIZE));

	int currencyWidth = wood.getLocalBounds().width, currencyHeight = wood.getLocalBounds().height;

	wood.setOrigin(sf::Vector2f(currencyWidth / 2, currencyHeight / 2));
	stone.setOrigin(sf::Vector2f(currencyWidth / 2, currencyHeight / 2));

	int X = 200, Y = 60, dist = 80 + Y;
	wood.setPosition(sf::Vector2f(VIEW_X - X, -VIEW_Y + Y));
	stone.setPosition(sf::Vector2f(VIEW_X - X, -VIEW_Y + dist));
	
	// currencies texts
	font.loadFromFile("fonts/PublicPixel.ttf");

	textStone.setFont(font);
	textStone.setFillColor(sf::Color::White);
	textStone.setStyle(sf::Text::Bold);
	textStone.setCharacterSize(25);

	textWood.setFont(font);
	textWood.setFillColor(sf::Color::White);
	textWood.setStyle(sf::Text::Bold);
	textWood.setCharacterSize(25);

	sf::String woodString = std::to_string(woodAmount), stoneString = std::to_string(stoneAmount);
	textWood.setString(woodString);
	textStone.setString(stoneString);

	textWood.setOrigin(sf::Vector2f(textWood.getLocalBounds().width / 2, textWood.getLocalBounds().height / 2));
	textStone.setOrigin(sf::Vector2f(textStone.getLocalBounds().width / 2, textStone.getLocalBounds().height / 2));

	textWood.setPosition(sf::Vector2f(VIEW_X - (X - 65 - (woodString.getSize() * 8)), -VIEW_Y + Y));
	textStone.setPosition(sf::Vector2f(VIEW_X - (X - 65 - (stoneString.getSize() * 8)), -VIEW_Y + dist));

}

int ShopState::update(std::vector<sf::Event> events, float dTime)
{
	return 0;
}

void ShopState::draw()
{
	window.clear(sf::Color::Black);

	window.draw(menuSprite);

	window.draw(wood);
	window.draw(textWood);
	window.draw(stone);
	window.draw(textStone);
}
