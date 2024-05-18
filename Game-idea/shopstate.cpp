#include "shopstate.hpp"

ShopState::ShopState(sf::RenderWindow& inWindow)
	: State(inWindow)
{
	int VIEW_X = CON::VIEW_SIZE_X / 2, VIEW_Y = CON::VIEW_SIZE_Y / 2;
	//menu
	menuTexture.loadFromFile("textures/shop/shop.png");
	menuSprite.setTexture(menuTexture);
	menuSprite.setScale(CON::PIXEL_SIZE, CON::PIXEL_SIZE);
	menuSprite.setPosition(-VIEW_X, -VIEW_Y);

	//currencies
	goldTexture.loadFromFile("textures/currencies/gold.png");

	gold.setTexture(goldTexture);
	gold.setScale(sf::Vector2f(CON::PIXEL_SIZE, CON::PIXEL_SIZE));

	int currencyWidth = gold.getLocalBounds().width, currencyHeight = gold.getLocalBounds().height;

	gold.setOrigin(sf::Vector2f(currencyWidth / 2, currencyHeight / 2));

	int X = 200, Y = 60, dist = 80 + Y;
	gold.setPosition(sf::Vector2f(VIEW_X - X, -VIEW_Y + Y));
	
	// currencies texts
	font.loadFromFile("fonts/PublicPixel.ttf");

	textGold.setFont(font);
	textGold.setFillColor(sf::Color::White);
	textGold.setStyle(sf::Text::Bold);
	textGold.setCharacterSize(25);


	sf::String woodString = std::to_string(goldAmount);
	textGold.setString(woodString);

	textGold.setOrigin(sf::Vector2f(0, textGold.getLocalBounds().height / 2));

	textGold.setPosition(sf::Vector2f(VIEW_X - (X - 45), -VIEW_Y + Y));

	// card
	cardTexture.loadFromFile("textures/shop/card.png");
	card.setTexture(cardTexture);
	card.setScale(sf::Vector2f(CON::PIXEL_SIZE, CON::PIXEL_SIZE));
	card.setPosition(sf::Vector2f(-VIEW_X + 250, -VIEW_Y + 595));

	// weapon
	weaponTexture.loadFromFile("textures/shop/weapon.png");
	weapon.setTexture(weaponTexture);
	weapon.setScale(sf::Vector2f(CON::PIXEL_SIZE, CON::PIXEL_SIZE));
	weapon.setPosition(sf::Vector2f(card.getPosition().x + 23, card.getPosition().y + 18));

	// buy
	buyTexture.loadFromFile("textures/shop/buy.png");
	buy.setTexture(buyTexture);
	buy.setScale(sf::Vector2f(CON::PIXEL_SIZE, CON::PIXEL_SIZE));
	buy.setPosition(sf::Vector2f(card.getPosition().x + 12, card.getPosition().y + card.getGlobalBounds().height - 80));

	// texts
	weaponTitle.setFont(font);
	weaponTitle.setFillColor(sf::Color::Black);
	weaponTitle.setStyle(sf::Text::Bold);
	weaponTitle.setCharacterSize(20);
	weaponTitle.setString("Knife");
	weaponTitle.setOrigin(sf::Vector2f(weaponTitle.getLocalBounds().width / 2, 0));
	weaponTitle.setPosition(card.getPosition() + sf::Vector2f(card.getGlobalBounds().width / 2 , 140));

	weaponDetails.setFont(font);
	weaponDetails.setStyle(sf::Text::Bold);
	weaponDetails.setFillColor(sf::Color::Black);
	weaponDetails.setCharacterSize(18);
	weaponDetails.setString("Damage\nFirerate\nReload\nMagasize\nRange\nAccuracy");
	weaponDetails.setLineSpacing(1.6);
	weaponDetails.setPosition(card.getPosition() + sf::Vector2f(20, 190));

	weaponDetailValues.setFont(font);
	weaponDetailValues.setStyle(sf::Text::Bold);
	weaponDetailValues.setFillColor(sf::Color::Black);
	weaponDetailValues.setCharacterSize(18);
	weaponDetailValues.setString("1\n1s\n1s\n1\n1m\n100°");
	weaponDetailValues.setLineSpacing(1.6);
	weaponDetailValues.setOrigin(weaponDetailValues.getLocalBounds().width, 0);
	weaponDetailValues.setPosition(card.getPosition() + sf::Vector2f(card.getGlobalBounds().width - 17,  190));
	
}

int ShopState::update(std::vector<sf::Event> events, float dTime)
{

	for (const auto& e : events) {
		if (e.type == sf::Event::Closed)
			window.close();
	}

	return 0;
}

void ShopState::draw()
{
	window.clear(sf::Color::Black);

	window.draw(menuSprite);

	window.draw(gold);
	window.draw(textGold);

	window.draw(card);
	window.draw(buy);
	window.draw(weapon);

	window.draw(weaponTitle);
	window.draw(weaponDetails);
	window.draw(weaponDetailValues);
}
