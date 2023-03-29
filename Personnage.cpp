
#include "Personnage.h"
#include "Reseau.h"
#include "Hidden_neurone.h"
#include "wincodec.h"
#include <random>
#include "utils.h"
#include <iostream>

using namespace std;

extern int NbreEnemy;
extern IWICImagingFactory* pIWICFactory;
extern ID2D1HwndRenderTarget* pRenderTarget;
extern zGlobals* global_ptr;
Joueur::Joueur() : m_x(0), m_y(0), m_reward(0), m_previous_reward(0), m_previous_stage(0)
{
	Joueur::create_Reseau();

}
void Joueur::Reset_joueur(Reseau* reseau)
{
	printf("Joueur is being created");
	m_x = 0;
	m_y = 0;
	m_reward = 0;
	m_previous_reward = 0;
	m_previous_stage = 0;
	//détruire l'ancien réseau
	m_reseau = new Reseau(reseau->get_n_Hidden_layer(), reseau->get_n_inputs(), reseau->get_n_Hidden_neurones(), reseau->get_Hidden_layers(), reseau->get_inputs(), reseau->get_outputs(), this);
	printf("Reseau created");
}

void Joueur::move(Dir Direction)
{
	//Faut changer ça en méthode pour bouger dans un direction
	press(Direction.dir, 0);
	Sleep(10);
	press(Direction.dir, 1);
}



void Joueur::update()
{
	m_reward = m_previous_reward + global_ptr->time_in_stage;
	if (global_ptr->stage_num > m_previous_stage)
	{
		m_previous_reward = global_ptr->time_in_stage;
		m_previous_stage = global_ptr->stage_num;
	}
	//Joueur::set_scaling();
	m_x = get_player_pos().x;
	m_y = get_player_pos().y;
	m_reseau->update();
	//Joueur::move_peur(&x_p, &y_p);
	
	
}
void Joueur::create_Reseau() {
	m_reseau = new Reseau(1, 1, 1, this);
	//printf("Reseau created");
}

float Joueur::get_x() const
{
	return m_x;
}

float Joueur::get_y() const
{
	return m_y;
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
	//printf("Joueur virtuel mort");
}

void Joueur::set_pos(double x, double y)
{
	m_x = x;
	m_y = y;
}

void Joueur::set_reward(int i)
{
	m_reward = 0;
}