#pragma once
#include "utils.h"
#include <vector>


struct EnemySimplified {
	int32_t id;
	int32_t prev_life;
};


class InputHelper {
public:
	//Methods:
	InputHelper();
	void updateInputs();
	std::vector<double> getInputs();
	void AddBullet();
	void AddEnemy();
	void CheckForPotentialDamage(zEnemy* enemy);
	Pos getPlayerPos();
	zBullet* getBulletsData();

	zEnemyList* getEnemiesData();


	//Member variables, public because. : 
	std::vector<double> inputs;
	std::vector<EnemySimplified> enemies;
	zBullet* bullets;
	zEnemyList* enemy_list;
	size_t nearestEnemy_id;
	size_t previous_total_damage_including_ignored;
	Pos playerPos;
	const size_t numBullets = 2000;
	bool isDealingDamage;
	//const int numEnemies = 200; idk the number actually
	//zEnemy enemies; i will implement that later
private:
};