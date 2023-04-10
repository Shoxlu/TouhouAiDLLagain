
#include "Personnage.h"
#include "Reseau.h"
#include "Hidden_neurone.h"
#include <random>
#include "utils.h"
#include <iostream>

using namespace std;

extern int NbreEnemy;
extern zGlobals* global_ptr;


Joueur::Joueur() :m_pos(Pos{ 0, 0 }), m_reward(0), m_previous_reward(0), m_previous_stage(0)
{
	m_reseau = new Reseau(1, 1, 1, this);
}
void Joueur::Reset_joueur(Reseau* reseau)
{
	m_reward = 0;
	m_previous_reward = 0;
	m_previous_stage = 0;
	m_reseau = new Reseau(reseau->get_n_Hidden_layer(), reseau->get_n_inputs(), reseau->get_n_Hidden_neurones(), reseau->get_Hidden_layers(), reseau->get_inputs(), reseau->get_outputs(), this);
}

void Joueur::move(Dir Direction, bool release)
{
	press(Direction.dir, release);
}



void Joueur::update()
{
	m_reward = m_previous_reward + global_ptr->time_in_stage;
	if (global_ptr->stage_num > m_previous_stage)
	{
		m_previous_reward = global_ptr->time_in_stage;
		m_previous_stage = global_ptr->stage_num;
	}
	m_pos = get_player_pos();
	m_reseau->update();
}

Pos Joueur::get_pos()
{
	return m_pos;
}

int Joueur::get_reward()
{
	return m_reward;
}

Reseau* Joueur::get_reseau()
{
	return m_reseau;
}

Joueur::~Joueur()
{
}

void Joueur::set_pos(double x, double y)
{
	m_pos.x = x;
	m_pos.y = y;
}

void Joueur::set_reward(int i)
{
	m_reward = 0;
}