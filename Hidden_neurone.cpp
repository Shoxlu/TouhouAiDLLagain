#include "pch.h"
#include "Input.h"
#include "Hidden_neurone.h"
#include "Personnage.h"
#include <stdio.h>
#include <iostream>

Hidden_neurone::~Hidden_neurone()
{

}
Hidden_neurone::Hidden_neurone(int n_Inputs, GenericNeurone** Inputs, Joueur* joueur) : m_n_Inputs(n_Inputs), m_joueur(joueur), m_input(0.0f), m_weight(0.7), m_output(0), m_Inputs(Inputs), m_Active(0)
{
	m_weights = new float[n_Inputs];
	for (int i = 0; i < n_Inputs; i++) {
		m_weights[i] = 0.0;
	}
}
Hidden_neurone::Hidden_neurone(int n_Inputs, GenericNeurone** Inputs, float* weights, Joueur* joueur) : m_n_Inputs(n_Inputs), m_joueur(joueur), m_input(0.0f), m_weight(0), m_output(0), m_Inputs(Inputs), m_Active(0)
{
	m_weights = new float[n_Inputs];
	for (int i = 0; i < n_Inputs; i++) {
		m_weights[i] = weights[i];
	}
}
int Hidden_neurone::get_n_Inputs() const
{
	return m_n_Inputs;
}
float* Hidden_neurone::get_weights()
{
	return m_weights;
}
GenericNeurone* Hidden_neurone::get_Input(int i) {
	return m_Inputs[i];
}

void Hidden_neurone::mutation_hidden_neurone()
{
	for (int i = 0; i < m_n_Inputs; i++)
	{
		if (random_float_positive() > 0.5)
		{
			m_weights[i] += random_float() * 0.2;
		}
	}
}
void Hidden_neurone::change_weights(int n_inputs)
{
	if (m_n_Inputs != n_inputs)
	{
		int debut = m_n_Inputs;
		int fin = n_inputs;
		if (debut > fin)
		{
			fin = m_n_Inputs;
			debut = n_inputs;
		}
		auto weights = new float[n_inputs];
		
		for (int i = 0; i < m_n_Inputs; i++) {
			weights[i] = m_weights[i];
		}
		for (int i = debut; i < fin; i++)
		{
			weights[i] = 1;
		}
		m_weights = weights;
	}
}
void Hidden_neurone::mutation_Input()
{
	return;
}
void Hidden_neurone::delete_Inputs(int j)
{
	delete m_Inputs[j];
	if (j != 0)
	{
		GenericNeurone* temp_personnages = m_Inputs[0];
		m_Inputs[0] = m_Inputs[j];
		m_Inputs[j] = temp_personnages;
	}
	for (int k = 0; k < m_n_Inputs - 1; k++)
	{
		m_Inputs[k] = m_Inputs[k + 1];
	}
	m_n_Inputs -= 1;
}
void Hidden_neurone::Activation()
{
	m_input = 0;
	m_Active = 0;
	for (int i = 0; i < m_n_Inputs; i++)
	{
		auto input = m_Inputs[i]->get_output();
		m_input = m_input + input * m_weights[i];
		//printf("Hidden_n input: %f, weight : %f, m_n_Inputs : %i \n", m_input, *m_weights[i], i);
	}
	m_output = 1/(1+pow(2.71828, -m_input));
	if (m_output > 0.5)
	{
		m_output = 1;
		m_Active = 1;
		return;
	}
	m_output = 0;
}

void Hidden_neurone::change_input(GenericNeurone** Inputs, int n_inputs)
{
	m_n_Inputs = n_inputs;
	m_Inputs = Inputs;
}

float Hidden_neurone::get_output()
{
	return m_output;
}

bool Hidden_neurone::isActive()
{
	return m_Active;
}

GenericNeurone** Hidden_neurone::get_Inputs() {
	return m_Inputs;
}