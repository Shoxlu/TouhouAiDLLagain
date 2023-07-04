#include "Joueur.h"
#include "NeuralNetwork.h"
#include "utils.h"

Joueur::Joueur() :m_pos(Pos{ 0, 0 }), m_reward(1), m_previous_reward(1), m_previous_stage(1), m_reseau(nullptr)
{
	inputHelper = pinputHelper;
	moves[0] = Dir{ Up };
	moves[1] = Dir{ Down };
	moves[2] = Dir{ Right };
	moves[3] = Dir{ Left };
	moves[4] = Dir{ Focus };
}

Joueur::~Joueur() {
	if (m_reseau)
		delete m_reseau;
}

void Joueur::CreateNetwork() {
	std::vector<int> baseNodes = { INPUTS_MAX, 5 };
	m_reseau = new NeuralNetwork(baseNodes[0], baseNodes[1]);
	//m_reseau = new NeuralNetwork(layerSizes);
}

void Joueur::Reset()
{
	m_reward = 0;
	m_previous_reward = 0;
	m_previous_stage = 0;
}

void Joueur::move(std::vector<double> outputs)
{
	//ReleaseAllInputs();
	*Inputs |= Shoot;
	for (size_t i = 0; i < outputs.size(); i++) {
		if (outputs[i] > 0.5) {
			*Inputs |= moves[i].key;
		}
	}
}

void Joueur::update()
{
	m_reward = m_previous_reward + global_ptr->time_in_stage;
	if (global_ptr->stage_num > m_previous_stage)
	{
		m_previous_reward = m_reward;
		m_previous_stage = global_ptr->stage_num;
	}
	std::vector<double> outputs = m_reseau->CalculateOutputs(pinputHelper->getInputs());
	move(outputs);
}