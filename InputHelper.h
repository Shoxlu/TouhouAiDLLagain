#pragma once
#include "utils.h"
#include <vector>

class InputHelper {
public:
	//Methods:
	InputHelper();
	void updateInputs();
	std::vector<double> getInputs();
	void AddBullet();
	void AddEnemy();
	Pos getPlayerPos();
	zBullet* getBulletsData();

	zEnemyList* getEnemiesData();


	//Member variables, public because. : 
	std::vector<double> inputs;
	zBullet* bullets;
	zEnemyList* enemy_list;
	Pos playerPos;
	const size_t numBullets = 2000;
	//const int numEnemies = 200; idk the number actually
	//zEnemy enemies; i will implement that later
private:
};