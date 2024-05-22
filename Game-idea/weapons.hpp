#pragma once
#include <vector>
#include "SFML/System.hpp"
#include <array>

struct Bullet {
	sf::Vector2<sf::Int64> pos = sf::Vector2<sf::Int64>(0,0);
	int angle = 0;
	char type = 0;
	int speed = 0;
};

struct Weapon {
	void shoot(int angle, std::vector<Bullet>& bullets) {};

	std::string name = "";
	char bullet = 0;
	int nBullets = 1;

	unsigned char damageIndex = 0;
	std::vector<sf::Uint16> damage;

	unsigned char firerateIndex = 0;
	std::vector<sf::Uint16> firerate;

	unsigned char reloadIndex = 0;
	std::vector<sf::Uint16> reload;

	unsigned char magazineIndex = 0;
	std::vector<sf::Uint16> magazine;

	unsigned char rangeIndex = 0;
	std::vector<sf::Uint16> range;
	
	unsigned char speedIndex = 0; // projectile speed
	std::vector<sf::Uint16> speed;
	
	unsigned char weightIndex = 0;
	std::vector<sf::Uint16> weight;
	
	unsigned char accuracyIndex = 0;
	std::vector<sf::Uint16> accuracy;
};

class WeaponsManager {
public:
	WeaponsManager() {
		Weapon w;
		w.name = "Pistol";
		w.damage = { 80, 90, 100, 110 };
		w.firerate = { 1000, 800, 700, 600 }; // ms
		w.reload = { 3000, 2500, 2000, 1000 }; // ms
		w.magazine = { 8, 10, 12, 16 };
		w.range = { 600, 650, 700, 750 }; // px
		w.speed = { 10, 11, 12, 13, 15 }; // px/ms
		w.weight = { 900, 950, 1000 }; // percent of your potential speed
		w.accuracy = { 30, 25, 20 };
		weapons.push_back(w);

		w = Weapon();
		w.name = "NoceGun";
		w.damage = { 80, 90, 100, 110 };
		w.firerate = { 600, 800, 700, 0 }; // ms
		w.reload = { 3000, 2500, 2000, 1000 }; // ms
		w.magazine = { 8, 10, 12, 16 };
		w.range = { 600, 650, 700, 750 }; // px
		w.speed = { 10, 11, 12, 13, 15 }; // px/ms
		w.weight = { 900, 950, 1000 }; // percent of your potential speed
		w.accuracy = { 30, 25, 20 };
		weapons.push_back(w);
	};

	std::vector<Weapon> weapons;
	std::vector<Bullet> bullets;
	unsigned char currentWeapon = 0;
};