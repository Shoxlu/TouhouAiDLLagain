#pragma once
#include "utils.h"
#include "InputHelper.h"
struct Dir;
class NeuralNetwork;
class Joueur
{
public:
	Joueur();
	~Joueur();
	void CreateNetwork();
	void Reset();
	void update();
	bool didDamage();
	bool isBombing();
	bool isUnderEnemy(double enemy_x);
	//bool isUnderEnemy();
	bool isNotAway();
	void move(std::vector<double> outputs);
	Pos m_pos;
	int32_t m_reward;
	int32_t m_previous_reward;
	int32_t m_previous_stage;
	int32_t prev_bombs;
	NeuralNetwork* m_reseau;
	InputHelper* inputHelper;
	Dir moves[6];
	bool isElite;
private:
};