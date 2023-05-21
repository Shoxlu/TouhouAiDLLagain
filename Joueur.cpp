
#include "Joueur.h"
#include "NeuralNetwork.h"
#include <random>
#include "utils.h"
#include <iostream>

#define VK_W 0x57
#define VK_R 0x52
#define VK_D 0x44
#define VK_C 0x43
#define VK_X 0x58

using namespace std;

extern int NbreEnemy;
extern zGlobals* global_ptr;
extern InputHelper* pinputHelper;

Joueur::Joueur() :m_pos(Pos{ 0, 0 }), m_reward(0), m_previous_reward(0), m_previous_stage(0)
{
	inputHelper = pinputHelper;
	int layerSizes[4] = { 2+2000*6, 10, 10, 9};
	m_reseau = new NeuralNetwork(layerSizes, 4);
	moves[0] = Dir{ {VK_UP}, 1, 1 };
	moves[1] = Dir{ {VK_DOWN}, 1, 1 };
	moves[2] = Dir{ {VK_RIGHT}, 1, 1};
	moves[3] = Dir{ {VK_LEFT}, 1, 1 };
	moves[4] = Dir{ {VK_X}, 1, 1 };
	moves[5] = Dir{ {VK_UP, VK_RIGHT }, 2, 1};
	moves[6] = Dir{ {VK_DOWN, VK_RIGHT},2, 1 };
	moves[7] = Dir{ {VK_UP, VK_LEFT},2, 1 };
	moves[8] = Dir{ {VK_DOWN,VK_LEFT }, 2, 1 };
}

Joueur::~Joueur() {
	if (m_reseau)
		delete m_reseau;
}

void Joueur::Reset(NeuralNetwork* reseau)
{
	m_reward = 0;
	m_previous_reward = 0;
	m_previous_stage = 0;
	m_reseau = reseau;
	m_reseau->mutation();
		//new Reseau(reseau->get_n_Hidden_layer(), reseau->get_n_inputs(), reseau->get_n_Hidden_neurones(), reseau->get_Hidden_layers(), reseau->get_inputs(), reseau->get_outputs(), this);
}

void Joueur::move(int output)
{
	ReleaseAllInputs();
	for (int i = 0; i < moves[output].n_dir; i++)
	{
		press(moves[output].dir[i], 0);
	}
	//press(VK_W, 0);
}

int Joueur::update()
{
	m_reward = m_previous_reward + global_ptr->time_in_stage;
	if (global_ptr->stage_num > m_previous_stage)
	{
		m_previous_reward = global_ptr->time_in_stage;
		m_previous_stage = global_ptr->stage_num;
	}
	double* input = inputHelper->getInputs();
	int output = m_reseau->Classify(input);
	move(output);
	return output;
}
double normalize(double angle) {
	while (angle > 2 * 3.141592653) {
		angle -= 2 * 3.141592653;
	}
	while (angle < 0) {
		angle += 2 * 3.141592653;
	}
	return angle;
}
double rad(double x) {
	return  normalize(x * 3.141592653 / 180.0);
}

double angleToplayer(double x, double y,double y2) {
	return acos(abs(y2)/hypot(x, y));
}

void Joueur::update_()
{
	ReleaseAllInputs();
	m_pos = get_player_pos();
	zBullet* bullet = bulletNear(m_pos.x, m_pos.y);
	if (!bullet)
		return;
	double angle_to_go = normalize(bullet->angle + 1.57079633 / 2);
	
	int moves_[8][3] = { {VK_DOWN,0,0}, {VK_UP,0,180},{VK_RIGHT,0, 90},{VK_LEFT,0,270},{VK_DOWN, VK_RIGHT,315},{VK_DOWN, VK_LEFT,225},{VK_UP, VK_RIGHT,45}, {VK_UP, VK_LEFT,135}};
	double angle_to_player = normalize(angleToplayer(bullet->pos.x-m_pos.x, bullet->pos.y-m_pos.y, bullet->pos.y-m_pos.y)+rad(90));
	for (int i = 0; i < 8; i++)
	{
		
		if (angle_to_player > rad(moves_[i][2]-22.5) && angle_to_player <= rad(moves_[i][2] + 22.5)) {
			if(abs(bullet->angle - angle_to_player) <= rad(10))
			{ 
				press(moves_[i][0], 0);
				printf("move");
				if (moves_[i][1] != 0) {
					press(moves_[i][1], 0);
					printf("move");
				}
			}
			
		}
	}
	
}