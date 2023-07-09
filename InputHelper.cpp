#include "InputHelper.h"
#include <stdio.h>

InputHelper::InputHelper() : enemy_list(nullptr), bullets(), playerPos(Pos{ 0, 0 }), nearestEnemy_id(-1), isDealingDamage(false), previous_total_damage_including_ignored(0)
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
	//Probably gonna replace all of that by some "assign to a place a thing", meaning i'll divide an area around the player into smaller places and assign them a value based on the thing that is present on the place.
	updateInputs();
	AddBullet();
	AddEnemy();
	//AddLaser(); -> future implementation
	inputs[INPUTS_MAX-2] = playerPos.x/192.0;
	inputs[INPUTS_MAX-1] = (playerPos.y-224.0)/224.0;
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
			inputs[0] = bullet.pos.x / 192.0;
			inputs[1] = (bullet.pos.y - 224.0) / 224.0;
			inputs[2] = (bullet.hitbox_radius-16)/16.0;
			inputs[3] = (bullet.angle - M_PI) / M_PI;
			inputs[4] = (distance-290)/290;
			inputs[5] = atan2(bullet.pos.y - playerPos.y, bullet.pos.x - playerPos.x) / M_PI;
			inputs[6] = (bullet.speed-5)/5;
			lastDistance = distance;
		}
	}
	if (lastDistance == 10000000) {
		inputs[0] = 0;
		inputs[1] = 0;
		inputs[2] = 0;
		inputs[3] = 0;
		inputs[4] = 0;
		inputs[5] = 0;
		inputs[6] = 0;
	}
}
void InputHelper::AddEnemy() {
	double lastDistance = 10000000;
	zEnemy* enemy = nullptr;
	for (zEnemyList* enemy_list_iterator = enemy_list; enemy_list != nullptr; enemy_list = enemy_list->next) {
		enemy = enemy_list->entry;
		double distance = sqrt(pow(enemy->data.abs_pos.pos.x- playerPos.x, 2) + pow(enemy->data.abs_pos.pos.y- playerPos.y, 2));
		if (distance < lastDistance && enemy->data.abs_pos.pos.x != 0 && enemy->data.abs_pos.pos.y != 0) {
			inputs[7] = atan2(enemy->data.abs_pos.pos.x - playerPos.y, enemy->data.abs_pos.pos.y - playerPos.x) / M_PI;
			inputs[8] = enemy->data.abs_pos.pos.x/ 192.0;
			inputs[9] = (enemy->data.abs_pos.pos.y - 224.0) / 224.0;
			inputs[10] = (distance - 290) / 290;
			lastDistance = distance;
		}
		CheckForPotentialDamage(enemy);
	}
	if (lastDistance == 10000000) {
		inputs[7] = 0;
		inputs[8] = 0;
		inputs[9] = 0;
		inputs[10] = 0;
	}
}


void InputHelper::CheckForPotentialDamage(zEnemy* enemy) {
	for (size_t i = 0; i < enemies.size(); i++) {
		isDealingDamage = false;
		if (enemy->enemy_id == enemies[i].id) {
			if (enemies[i].prev_life > enemy->data.life.current) {
				enemies[i].prev_life = enemy->data.life.current;
				isDealingDamage = true;
			}
			return;
		}
	}
	enemies.emplace_back(EnemySimplified{ enemy->enemy_id, enemy->data.life.current });
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

//zLaser InputHelper::AddClosestPointToLaser() {
//		//for every lasers:
//		//check its type:
//		//if it's curvy: iterate the nodes and find the closest, if it's infinite: caculate the area and the closest point, if it's line, do the same
//}