#include "InputHelper.h"
#include <corecrt_math.h>
#include <stdio.h>

InputHelper::InputHelper() : bullets(), playerPos(Pos{ 0, 0 }), numInputs(2 + 2000 * 6), numCurBullets(0)
{
	inputs = new double[2 + 2000 * 6];
}

InputHelper::~InputHelper()
{
	if (inputs) {
		delete[] inputs;
	}
}

void InputHelper::updateInputs()
{
	playerPos = getPlayerPos();
	bullets = getBulletsData();
}
double* InputHelper::getInputs(){
	numInputs = 0;
	updateInputs();
	
	int a = 0;
	numCurBullets = 0;
	for (int i = 0; i < 2000*6; i+=6) {
		//printf("State : %d", bullets[i / 6].state).
		zBullet bullet = bullets[i / 6];
		if (bullet.state != 1 ) {
			inputs[i - a] = -10000;
			inputs[i + 1 - a] = 0;
			inputs[i + 2 - a] = 0;
			inputs[i + 3 - a] = 0;
			inputs[i + 4 - a] = 0;
			inputs[i + 5 - a] = 0;
			a += 6;
			continue;
		}
		inputs[i-a] = bullet.pos.x;//todo: add proportionnality
		inputs[i + 1 - a] = bullet.pos.y ;
		/*printf("x=%f ", bullets[i / 6].pos.x);
		printf("y=%f ", bullets[i / 6].pos.y);
		printf("z=%f ", bullets[i / 6].pos.z);
		printf("x_test=%f ", bullets[(i-2) / 6].pos.x);
		printf("y_test=%f ", bullets[(i-2) / 6].pos.y);
		printf("z_test=%f ", bullets[(i-2) / 6].pos.z);*/
		inputs[i+2-a] = bullet.angle;
		//printf("angle=%f ", bullets[i / 6].angle);
		inputs[i+3-a] = bullet.speed;
		//printf("speed=%f ", bullets[i / 6].speed);
		inputs[i+4-a] = bullet.hitbox_radius ;
		//printf("hitbox=%f ", bullets[i / 6].hitbox_radius);
		inputs[i+5-a] = sqrt(pow(inputs[i-a], 2) + pow(inputs[i+1-a], 2));
		//printf("distance = %f ", inputs[i + 5 - a]);
		//printf("\n");
		numInputs += 6;
		numCurBullets += 1;
	}
	inputs[2000*6] = playerPos.x;
	inputs[2000*6+1] = playerPos.y;
	return inputs;
}
Pos InputHelper::getPlayerPos()
{
	return get_player_pos();//probably do something better lol
}

zBullet* InputHelper::getBulletsData()
{
    zBulletManager *Bullet_PTR = *(zBulletManager**)0x4CF2BC;
	
	return Bullet_PTR->bullets;
}
