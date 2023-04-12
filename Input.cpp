
#include "Input.h"
#include "Personnage.h"
#include <iostream>

Input::~Input()
{
	//m_sprite->Release();
}
Input::Input(double x, double y,int n_outputs, Joueur* joueur) : m_offset(Pos{ x, y }), m_joueur(joueur), m_isActive(0), m_pos(Pos{ x, y }), m_input(0), m_output(0), m_type(0), m_n_outputs(n_outputs)
{
	m_weights = create_weights(n_outputs, 1.0, randint(0, n_outputs - 1));
}
Input::Input(double x, double y, int n_outputs, float* weights, Joueur* joueur) : m_offset(Pos{ x, y }), m_joueur(joueur), m_isActive(0), m_pos(Pos{ x, y }), m_input(0), m_output(0), m_type(0), m_n_outputs(n_outputs)
{
	copy_weights(n_outputs, &m_weights, weights);
}

float Input::get_input() const
{
	return m_input;
}
float Input::get_output(int k, int self_id)//self_id is useless but well
{
	//printf("Input output: %f, k: %d, hidden_neurone_id: %d\n", m_isActive * m_weights[k], k, self_id);
	return m_isActive * m_weights[k];
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

float* Input::get_weights()
{
	return m_weights;
}

void Input::mutation_Input()
{
	if (random_float() > 0.4)
	{
		m_offset.x += random_float() * 50;
		m_offset.y += random_float() * 50;
	}
	for (int i = 0; i < m_n_outputs; i++)
	{
		if (random_float() > 0.4)
		{
			m_weights[i] += random_float() * 0.25;
		}
	}
}

void Input::change_weights(int n_outputs)
{
	if (m_n_outputs != n_outputs)
	{
		int debut = m_n_outputs;
		int fin = n_outputs;
		if (debut > fin)
		{
			fin = m_n_outputs;
			debut = n_outputs;
		}
		float* weights = new float[fin];
		for (int i = 0; i < debut; i++) {
			weights[i] = m_weights[i];
			printf("weights[%d]: %f \n", i, weights[i]);
		}
		for (int i = debut; i < fin; i++)
		{
			weights[i] = 0.0;
			printf("weights[%d]: %f \n", i, weights[i]);
		}
		m_weights = new float[fin];
		m_weights = weights;
	}
}

int Input::get_n_outputs()
{
	return m_n_outputs;
}

void Input::change_input(GenericNeurone** Inputs, int n_inputs)
{
}

void Input::Update()
{
	//le Input doit garder la même position relative à son joueur
	move();
	m_isActive = check_state();//Détecte s'il doit être allumé ou éteint
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