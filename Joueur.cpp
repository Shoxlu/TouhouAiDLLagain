#include "Joueur.h"
#include "NeuralNetwork.h"
#include "utils.h"

Joueur::Joueur() :m_pos(Pos{ 0, 0 }), m_reward(1), m_previous_reward(1), m_previous_stage(1), m_reseau(nullptr), prev_bombs(0)
{
	inputHelper = pinputHelper;
	moves[0] = Dir{ Up };
	moves[1] = Dir{ Down };
	moves[2] = Dir{ Right };
	moves[3] = Dir{ Left };
	moves[4] = Dir{ Focus };
	moves[5] = Dir{ Bomb };
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
	m_reward = m_previous_reward + m_reward + 1 + isBombing() * -300 + didDamage() * 1;
	m_previous_reward = global_ptr->graze_counter * 1.3;
	if (m_reward < 0)
		m_reward = 0;
	if (global_ptr->stage_num > m_previous_stage)
	{
		m_previous_reward = ceil(double(m_reward) *(1.0+double(global_ptr->time_in_stage)/50000.0)) + global_ptr->graze_counter * 1.3;// substract current_lives mutiplied with some stage factor
		m_previous_stage = global_ptr->stage_num;
	}
}

bool Joueur::didDamage() {
	if (pinputHelper->isDealingDamage)
		return true;
	return false;
}


bool Joueur::isBombing() {
	if (global_ptr->current_bombs < prev_bombs) {
		prev_bombs = global_ptr->current_bombs;
		return true;
	}
	prev_bombs = global_ptr->current_bombs;
	return false;
}

//bool Joueur::isUnderEnemy() {
//	if (m_pos.x <= enemy_x + 20 && m_pos.x >= enemy_x - 20)
//		return true;
//	return false;
//}

bool Joueur::isAtMiddleOfScreen() {

	if (m_pos.x < 130 && m_pos.x > -130 && m_pos.y > 224.0) {
		return true;
	}
	return false;
}
