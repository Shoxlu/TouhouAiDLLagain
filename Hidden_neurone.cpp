#include "pch.h"
#include "Input.h"
#include "Hidden_neurone.h"
#include "Personnage.h"
#include <stdio.h>
#include <iostream>

Hidden_neurone::~Hidden_neurone()
{

}
Hidden_neurone::Hidden_neurone(int n_Inputs, GenericNeurone** Inputs, Joueur* joueur) : m_n_Inputs(n_Inputs), m_joueur(joueur), m_input(0.0f), m_weight(1.0), m_output(0), m_Inputs(Inputs)
{
}
Hidden_neurone::Hidden_neurone(int n_Inputs, GenericNeurone** Inputs, float weight, Joueur* joueur) : m_n_Inputs(n_Inputs), m_joueur(joueur), m_input(0.0f), m_weight(weight), m_output(0), m_Inputs(Inputs)
{
}
int Hidden_neurone::get_n_Inputs() const
{
	return m_n_Inputs;
}
float Hidden_neurone::get_weight()
{
	return m_weight;
}
GenericNeurone* Hidden_neurone::get_Input(int i) {
	return m_Inputs[i];
}

void Hidden_neurone::mutation_hidden_neurone()
{
	if (randint(0, 100) < 10)
	{
		m_n_Inputs += 1;
		m_Inputs[m_n_Inputs - 1] = new Input(random_float() * 100, random_float() * 100, m_joueur);
	}
	else if(randint(0, 100) < 2 && m_n_Inputs > 1){
		delete_Inputs(randint(0, m_n_Inputs - 1));
	}
	if (randint(0, 100) < 20)
	{
		m_weight = m_weight + random_float()*0.2;
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
	m_output = 0;
	m_input = 0;
	for (int i = 0; i < m_n_Inputs; i++)
	{
		//std::cout << m_Inputs.input[i]->get_output() << "Hidden_neurone side, is_input\n";
		m_input += m_Inputs[i]->get_output() * m_weight;
	}
	
	if (m_input >= 1.0f)
	{
		m_output = m_weight;
		//printf("Activation output %f", m_output);
	}
}

float Hidden_neurone::get_output()
{
	return m_output;
}

bool Hidden_neurone::isActive()
{
	return m_input;
}

GenericNeurone** Hidden_neurone::get_Inputs() {
	return m_Inputs;
}