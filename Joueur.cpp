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
	std::vector<int> baseNodes = { INPUTS_MAX, OUTPUTS_MAX };
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
	*Inputs |= Shoot;
	for (size_t i = 0; i < outputs.size(); i++) {
		if (outputs[i] > 0) {
			*Inputs |= moves[i].key;
		}
	}
}

void Joueur::update()
{
	m_pos = get_player_pos();
	std::vector<double> inputs = pinputHelper->getInputs();
	move(m_reseau->CalculateOutputs(inputs));
	m_reward = m_previous_reward;
	m_previous_reward = m_reward;
	m_reward += global_ptr->time_in_stage*3 + global_ptr->graze_counter;
	if (global_ptr->stage_num > m_previous_stage)
	{
		m_previous_reward = m_reward;
		m_previous_stage = global_ptr->stage_num;
	}
}

bool Joueur::isAtMiddleOfScreen() {

	if (m_pos.x < 130 && m_pos.x > -130 && m_pos.y > 224.0) {
		return true;
	}
	return false;
}
