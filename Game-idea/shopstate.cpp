#include "shopstate.hpp"

ShopState::ShopState(sf::RenderWindow& inWindow, WeaponsManager& inWeaponsManager)
	: 
	State(inWindow),
	weaponsManager(inWeaponsManager)
{
	//menu
	menuTexture.loadFromFile("textures/shop/shop.png");
	menuSprite.setTexture(menuTexture);
	menuSprite.setScale(CON::PIXEL_SIZE, CON::PIXEL_SIZE);
	menuSprite.setPosition(-CON::VIEW_SIZE_X / 2, -CON::VIEW_SIZE_Y / 2);

	//currencies
	goldTexture.loadFromFile("textures/currencies/gold.png");
	gold.setTexture(goldTexture);
	gold.setScale(sf::Vector2f(CON::PIXEL_SIZE, CON::PIXEL_SIZE));

	int currencyWidth = gold.getLocalBounds().width, currencyHeight = gold.getLocalBounds().height;

	gold.setOrigin(sf::Vector2f(currencyWidth / 2, currencyHeight / 2));

	int X = 200, Y = 60, dist = 80 + Y;
	gold.setPosition(sf::Vector2f(CON::VIEW_SIZE_X / 2 - X, -CON::VIEW_SIZE_Y / 2 + Y));
	
	// currencies texts
	font.loadFromFile("fonts/PublicPixel.ttf");

	textGold.setFont(font);
	textGold.setFillColor(sf::Color::White);
	textGold.setStyle(sf::Text::Bold);
	textGold.setCharacterSize(25);


	sf::String woodString = std::to_string(goldAmount);
	textGold.setString(woodString);

	textGold.setOrigin(sf::Vector2f(0, textGold.getLocalBounds().height / 2));

	textGold.setPosition(sf::Vector2f(CON::VIEW_SIZE_X / 2 - (X - 45), -CON::VIEW_SIZE_Y / 2 + Y));

	// card
	cardTexture.loadFromFile("textures/shop/card.png");
	card.setTexture(cardTexture);
	card.setScale(sf::Vector2f(CON::PIXEL_SIZE, CON::PIXEL_SIZE));

	// weapon
	weaponTexture.loadFromFile("textures/shop/weapon.png");
	weapon.setTexture(weaponTexture);
	weapon.setScale(sf::Vector2f(CON::PIXEL_SIZE, CON::PIXEL_SIZE));

	// buy
	buyTexture.loadFromFile("textures/shop/buy.png");
	buy.setTexture(buyTexture);
	buy.setScale(sf::Vector2f(CON::PIXEL_SIZE, CON::PIXEL_SIZE));

	// texts
	weaponTitle.setFont(font);
	weaponTitle.setFillColor(sf::Color::Black);
	weaponTitle.setStyle(sf::Text::Bold);
	weaponTitle.setCharacterSize(20);

	weaponDetails.setFont(font);
	weaponDetails.setStyle(sf::Text::Bold);
	weaponDetails.setFillColor(sf::Color::Black);
	weaponDetails.setCharacterSize(18);
	weaponDetails.setString("Damage\nFirerate\nReload\nMagsize\nRange\nAccuracy");
	weaponDetails.setLineSpacing(1.6);

	weaponDetailValues.setFont(font);
	weaponDetailValues.setStyle(sf::Text::Bold);
	weaponDetailValues.setFillColor(sf::Color::Black);
	weaponDetailValues.setCharacterSize(18);
	weaponDetailValues.setLineSpacing(1.6);
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

	for (int i = 0; i < weaponsManager.weapons.size(); i++)
	{
		auto currentWeapon = weaponsManager.weapons[i];
		int VIEW_X = CON::VIEW_SIZE_X / 2, VIEW_Y = CON::VIEW_SIZE_Y / 2;
		card.setPosition(sf::Vector2f(-VIEW_X + 250 + i * 320, -VIEW_Y + 595));
		weapon.setPosition(sf::Vector2f(card.getPosition().x + 23, card.getPosition().y + 18));
		buy.setPosition(sf::Vector2f(card.getPosition().x + 12, card.getPosition().y + card.getGlobalBounds().height - 80));

		weaponTitle.setString(currentWeapon.name);
		weaponTitle.setOrigin(sf::Vector2f(weaponTitle.getLocalBounds().width / 2, 0));
		weaponTitle.setPosition(card.getPosition() + sf::Vector2f(card.getGlobalBounds().width / 2, 140));
		weaponDetails.setPosition(card.getPosition() + sf::Vector2f(20, 190));
			
		std::string firerateStr = std::to_string(1000.f / currentWeapon.firerate[currentWeapon.firerateIndex]);
		firerateStr = firerateStr.substr(0, firerateStr.find_first_of('.') + 2);

		weaponDetailValues.setString(
			std::to_string(currentWeapon.damage[currentWeapon.damageIndex]) + "\n" + 
			firerateStr + "s\n" +
			std::to_string(currentWeapon.reload[currentWeapon.reloadIndex] / 100) + "s\n" + 
			std::to_string(currentWeapon.magazine[currentWeapon.magazineIndex]) + "\n" + 
			std::to_string(currentWeapon.range[currentWeapon.rangeIndex] / 10) + "\n" + 
			std::to_string(currentWeapon.accuracy[currentWeapon.accuracyIndex]) + "°"
		);

		weaponDetailValues.setOrigin(weaponDetailValues.getLocalBounds().width, 0);
		weaponDetailValues.setPosition(card.getPosition() + sf::Vector2f(card.getGlobalBounds().width - 17, 190));

		window.draw(card);
		window.draw(buy);
		window.draw(weapon);

		window.draw(weaponTitle);
		window.draw(weaponDetails);
		window.draw(weaponDetailValues);
	}
}
