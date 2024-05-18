#pragma once
#include <vector>
#include "SFML/System.hpp"

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

	int damage = 0;
	int firerate = 0;
	int reload = 0;
	int magazine = 0;
	int range = 0;
	int speed = 0;
	int weight = 0;
	int accuracy = 0;
};

class Shotgun : public Weapon {
	Shotgun() {};
};