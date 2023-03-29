
#include "Input.h"
#include "Personnage.h"
#include <iostream>

Input::~Input()
{
	//m_sprite->Release();
}
Input::Input(double x, double y, Joueur* joueur) : m_offset(Pos{ x, y }), m_joueur(joueur), m_weight(1.0)
{
	Activation();
}
Input::Input(double x, double y, float weight, Joueur* joueur) : m_offset(Pos{ x, y }), m_joueur(joueur), m_weight(weight)
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
	if (randint(0, 100) < 20)
	{
		m_weight = m_weight + random_float() * 0.2;
	}
}

void Input::mutation_position()
{
	m_offset.x += random_float() * 20;
	m_offset.y += random_float() * 20;
}



void Input::Activation()
{
	//le Input doit garder la même position relative à son joueur
	move();
	m_output = check_state();//Détecte s'il doit être allumé ou éteint
}


void Input::move()
{
	m_pos.x = m_joueur->get_x() + m_offset.x;
	m_pos.y = m_joueur->get_y() + m_offset.y;
	/*m_pos_graphics.x = m_joueur->get_scaledCenter().x + m_offset.x;
	m_pos_graphics.y = m_joueur->get_scaledCenter().y + m_offset.y;*/
}

float Input::check_state()
{
	if (bulletNear(m_pos.x, m_pos.y))
	{
		return m_weight;
	}
	return 0;
}
