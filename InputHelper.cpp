#include "InputHelper.h"
#include <corecrt_math.h>
#include <stdio.h>

InputHelper::InputHelper() : bullets(), playerPos(Pos{ 0, 0 }), numInputs(2 + 2000 * 6), numCurBullets(0)
{
	inputs.assign(2 + 2000 * 6, 0);
}

InputHelper::~InputHelper()
{
}

void InputHelper::updateInputs()
{
	playerPos = getPlayerPos();
	bullets = getBulletsData();
}
std::vector<double> InputHelper::getInputs() {
	numInputs = 0;
	updateInputs();
	
	int a = 0;
	numCurBullets = 0;
	for (int i = 0; i < 2000*6; i+=6) {
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
		inputs[i-a] = bullet.pos.x/1000.0 / 192.0;//todo: add proportionnality
		inputs[i + 1 - a] = bullet.pos.y / 1000.0 /192.0 ;
		inputs[i+2-a] = bullet.angle / 1000.0 / (2*3.141592);
		inputs[i+3-a] = bullet.speed / 1000.0 / 10.0;
		inputs[i+4-a] = bullet.hitbox_radius / 1000.0 / 10.0;
		inputs[i+5-a] = sqrt(pow(inputs[i-a], 2) + pow(inputs[i+1-a], 2)) / 1000/ 590.050845;
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
