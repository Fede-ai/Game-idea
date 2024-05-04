#pragma once

struct GameInfo {
	//wood, stone, people, gold, food, water
	double resources[6] = { 0, 0, 0, 0, 0, 0 };
	int capacities[6] = { 10, 10, 10, 10, 10, 10 };
};