#pragma once
#include "utils.h"
#include <vector>


class InputHelper {
public:
	//Methods:
	InputHelper();
	~InputHelper();
	void updateInputs();
	std::vector<double> getInputs();
	void AddBullet(zBullet* bulletptr, int i, int a);
	Pos getPlayerPos();
	zBullet* getBulletsData();


	//Member variables, public because. : 
	std::vector<double> inputs;
	zBullet* bullets;
	Pos playerPos;
	int numInputs;//maybe it will be useful
	const int numBullets = 2000;
	int numCurBullets;
	//const int numEnemies = 200; idk the number actually
	//zEnemy enemies; i will implement that later
private:
};