#include "pch.h"
#include "Input.h"
#include "Hidden_neurone.h"
#include "Personnage.h"
#include <stdio.h>
#include <iostream>
#include "utils.h"

Hidden_neurone::~Hidden_neurone()
{

}
//Hidden_neurone::Hidden_neurone(int n_Inputs, GenericNeurone** Inputs, Joueur* joueur) : m_n_inputs(n_Inputs), m_joueur(joueur), m_input(0.0f), m_weight(0.7), m_output(0), m_inputs(Inputs), m_isActive(0)
//{
//	m_weights = create_weights(n_Inputs, 1, randint(0, n_Inputs-1));
//}
//Hidden_neurone::Hidden_neurone(int n_Inputs, GenericNeurone** Inputs, int n_weight, Joueur* joueur) : m_n_inputs(n_Inputs), m_joueur(joueur), m_input(0.0f), m_weight(0.7), m_output(0), m_inputs(Inputs), m_isActive(0)
//{
//	m_weights = create_weights(n_Inputs, 1, n_weight);
//} Actually obsolete
Hidden_neurone::Hidden_neurone(int n_inputs, GenericNeurone** Inputs, int n_outputs, float* weights, Joueur* joueur) :
m_n_inputs(n_inputs), m_joueur(joueur), m_input(0.0f), m_weight(0), m_output(0), m_inputs(Inputs), m_isActive(0), m_n_outputs(n_outputs)
{
	copy_weights(n_outputs, &m_weights, weights);
}
int Hidden_neurone::get_n_Inputs() const
{
	return m_n_inputs;
}
float* Hidden_neurone::get_weights()
{
	return m_weights;
}
GenericNeurone* Hidden_neurone::get_Input(int i) {
	return m_inputs[i];
}

void Hidden_neurone::mutation_hidden_neurone()
{
	for (int i = 0; i < m_n_inputs; i++)
	{
		if (random_float_positive() > 0.5)
		{
			m_weights[i] += random_float() * 0.25;
		}
	}
}
void Hidden_neurone::change_weights(int n_outputs)
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
		}
		for (int i = debut; i < fin; i++)
		{
			weights[i] = 0.0;
		}
		m_weights = new float[fin];
		m_weights = weights;
	}
}

int Hidden_neurone::get_n_outputs()
{
	return m_n_outputs;
}

void Hidden_neurone::mutation_Input()
{
	return;
}

void Hidden_neurone::change_input(GenericNeurone** Inputs, int n_inputs)
{
	m_n_inputs = n_inputs;
	m_inputs = Inputs;
}

float Hidden_neurone::get_output(int k, int self_id)
{
	m_input = 0;
	m_isActive = 0;
	for (int i = 0; i < m_n_inputs; i++)
	{
		auto input = m_inputs[i]->get_output(self_id, i); //i is here for future layers implementation (i think)
		m_input = m_input + input;
		//printf("Hidden_n input: %f, weight : %f, m_n_Inputs : %i \n", m_input, *m_weights[i], i);
	}
	m_output = 1 / (1 + pow(2.71828, -m_input));
	
	if (m_output > 0.5)
	{
		m_output = m_weights[k];;
		m_isActive = 1;
		//printf("Ouput = %f\n", m_output);
		return m_output;
	}
	m_output = 0;
	return m_output;
}

bool Hidden_neurone::isActive()
{
	return m_isActive;
}

GenericNeurone** Hidden_neurone::get_Inputs() {
	return m_inputs;
}