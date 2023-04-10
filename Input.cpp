
#include "Input.h"
#include "Personnage.h"
#include <iostream>

Input::~Input()
{
	//m_sprite->Release();
}
Input::Input(double x, double y, Joueur* joueur) : m_offset(Pos{ x, y }), m_joueur(joueur), m_weight(1), m_isActive(0)
{
	Activation();
}
Input::Input(double x, double y, float weight, Joueur* joueur) : m_offset(Pos{ x, y }), m_joueur(joueur), m_weight(weight), m_isActive(0)
{
	Activation();
}

float Input::get_input() const
{
	return m_input;
}
float Input::get_output()
{
	return m_output;
}

float Input::get_weight()
{
	return m_weight;
}
int Input::get_type() const
{
	return m_type;
}

Pos Input::get_pos() const
{
	return m_pos;
}
Pos Input::get_offset() const
{
	return m_offset;
}
Pos Input::get_pos_graphics() const
{
	return m_pos_graphics;
}

void Input::mutation_Input()
{
	if (randint(0, 100) < 40)
	{
		mutation_position();
	}
}

void Input::mutation_position()
{
	m_offset.x += random_float() * 100;
	m_offset.y += random_float() * 100;
}



void Input::Activation()
{
	//le Input doit garder la même position relative à son joueur
	move();
	m_output = check_state();//Détecte s'il doit être allumé ou éteint
}


void Input::move()
{
	Pos player_pos = m_joueur->get_pos();
	m_pos.x = player_pos.x + m_offset.x;
	m_pos.y = player_pos.y + m_offset.y;
}

float Input::check_state()
{
	if (bulletNear(m_pos.x, m_pos.y))
	{
		m_isActive = 1;
		return 1;
	}
	m_isActive = 0;
	return 0;
}

bool Input::isActive()
{
	return m_isActive;
}