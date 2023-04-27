
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
	int layerSizes[4] = { inputHelper->numInputs, 10, 10, 9};
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
	Release_All_Inputs();
	for (int i = 0; i < moves[output].n_dir; i++)
	{
		press(moves[output].dir[i], 0);
	}
	press(VK_W, 0);
}

void Joueur::update()
{
	m_reward = m_previous_reward + global_ptr->time_in_stage;
	if (global_ptr->stage_num > m_previous_stage)
	{
		m_previous_reward = global_ptr->time_in_stage;
		m_previous_stage = global_ptr->stage_num;
	}
	int output = m_reseau->Classify(inputHelper->getInputs(), inputHelper->numInputs);
	move(output);
	//m_reseau->update();
}
