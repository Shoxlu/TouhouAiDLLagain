#pragma once
#include "utils.h"


class InputHelper {
public:
	//Methods:
	InputHelper();
	~InputHelper();
	void updateInputs();
	double* getInputs();
	Pos getPlayerPos();
	zBullet* getBulletsData();


	//Member variables, public because. : 
	double* inputs;
	zBullet* bullets;
	Pos playerPos;
	int numInputs;//maybe it will be useful
	const int numBullets = 2000;
	//const int numEnemies = 200; idk the number actually
	//zEnemy enemies; i will implement that later
private:
};