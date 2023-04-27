#include "InputHelper.h"
#include <corecrt_math.h>

InputHelper::InputHelper() : bullets(), playerPos(Pos{ 0, 0 }), numInputs(2 + 2000 * 6)
{
	inputs = new double[2 + 2000 * 6];
}

InputHelper::~InputHelper()
{
}

void InputHelper::updateInputs()
{
	playerPos = getPlayerPos();
	bullets = getBulletsData();
}
double* InputHelper::getInputs(){
	numInputs = 0;
	updateInputs();
	inputs[0] = playerPos.x;
	inputs[1] = playerPos.y;
	int a = 0;
	for (int i = 2; i < 2000*6; i+=6) {
		if (bullets[i/6-2].state == 0) {
			i += 6;
			a += 6;
			continue;
		}
		inputs[i-a] = bullets[i/6-2].pos.x;
		inputs[i+1-a] = bullets[i/6-2].pos.y;
		inputs[i+2-a] = bullets[i / 6-2].angle;
		inputs[i+3-a] = bullets[i / 6-2].speed;
		inputs[i+4-a] = bullets[i / 6-2].hitbox_radius;
		inputs[i + 5] = sqrt(pow(inputs[i], 2) + pow(inputs[i + 1], 2));
		numInputs += 6;
	}
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
