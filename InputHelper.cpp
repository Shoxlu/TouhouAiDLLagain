#include "InputHelper.h"
#include <corecrt_math.h>
#include <stdio.h>
int NINPUTSPBULLET = 5;
constexpr auto N_BULLETS = 2000;
auto INPUTS_MAX = 2000 * NINPUTSPBULLET + 2;

InputHelper::InputHelper() : bullets(), playerPos(Pos{ 0, 0 }), numInputs(2 + 2000 * 6), numCurBullets(0)
{
	inputs.assign(INPUTS_MAX, 0);
}

InputHelper::~InputHelper()
{
}

void InputHelper::updateInputs()
{
	playerPos = getPlayerPos();
	bullets = getBulletsData();
	//enemies = getEnemiesData();
	//lasers = getLasersData();
}
std::vector<double> InputHelper::getInputs() {
	numInputs = 0;
	updateInputs();
	
	int a = 0;
	numCurBullets = 0;
	for (int i = 0; i < INPUTS_MAX; i+= NINPUTSPBULLET) {
		zBullet bullet = bullets[i / NINPUTSPBULLET];
		if (bullet.state != 1) {
			inputs[i - a] = -10000;
			a += NINPUTSPBULLET;
			continue;
		}
		AddBullet(&bullet, i, a);
		//AddEnemy(); -> future implementation
		//AddLaser(); -> future implementation
	}
	inputs[INPUTS_MAX-2] = playerPos.x;
	inputs[INPUTS_MAX-1] = playerPos.y;
	return inputs;
}

void InputHelper::AddBullet(zBullet* bulletptr, int i, int a) {
	zBullet bullet = *bulletptr;
	
	inputs[i - a] = bullet.pos.x / 192000.0;//todo: add proportionnality
	inputs[i + 1 - a] = bullet.pos.y / 192000.0;
	inputs[i + 2 - a] = bullet.angle / 6283.184;
	inputs[i + 3 - a] = bullet.hitbox_radius / 10000.0;
	inputs[i + 4 - a] = pow(inputs[i - a], 2) + pow(inputs[i + 1 - a], 2) / 590050.845;
	numCurBullets += 1;
	numInputs += NINPUTSPBULLET;
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
