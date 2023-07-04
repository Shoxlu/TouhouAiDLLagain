#include "InputHelper.h"
#include <stdio.h>

InputHelper::InputHelper() : enemy_list(nullptr), bullets(), playerPos(Pos{ 0, 0 })
{
	inputs.assign(INPUTS_MAX, 0);
}

void InputHelper::updateInputs()
{
	playerPos = getPlayerPos();
	bullets = getBulletsData();
	enemy_list = getEnemiesData();
	//lasers = getLasersData();
}
std::vector<double> InputHelper::getInputs() {
	updateInputs();
	AddBullet();
	AddEnemy();
	//AddLaser(); -> future implementation
	inputs[INPUTS_MAX-2] = playerPos.x/384.0 * 10;
	inputs[INPUTS_MAX-1] = playerPos.y/448.0 * 10;
	return inputs;
}

void InputHelper::AddBullet() {
	double lastDistance = 10000000;
	for (size_t i = 0; i < 2000; i++) {
		if (bullets[i].state != 1)
			continue;
		zBullet bullet = bullets[i];
		double distance = sqrt(pow(bullet.pos.x-playerPos.x, 2) + pow(bullet.pos.y- playerPos.y, 2));
		if (distance < lastDistance) {
			inputs[0] = bullet.pos.x/384.0 * 10;
			inputs[1] = bullet.pos.y/448.0 * 10;
			inputs[2] = bullet.angle/(M_PI*2) * 10;
			inputs[3] = distance/ 590.050845 * 10;
			lastDistance = distance;
		}
	}
}
void InputHelper::AddEnemy() {
	double lastDistance = 10000000;
	for (zEnemyList* enemy_list_iterator = enemy_list; enemy_list != nullptr; enemy_list = enemy_list->next) {
		zEnemy* enemy = enemy_list->entry;
		double distance = sqrt(pow(enemy->data.abs_pos.pos.x- playerPos.x, 2) + pow(enemy->data.abs_pos.pos.y- playerPos.y, 2));
		if (distance < lastDistance) {
			inputs[4] = enemy->data.abs_pos.pos.x / 384.0 *10;
			inputs[5] = enemy->data.abs_pos.pos.y / 448.0*10;
			inputs[6] = distance / 590.050845*10;
			lastDistance = distance;
		}
	}
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


zEnemyList* InputHelper::getEnemiesData()
{
	zEnemyManager* enemyPtr = *(zEnemyManager**)0x4cf2d0;
	
	return enemyPtr->active_enemy_list_head;
}
