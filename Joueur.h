

#include "utils.h"
#include "InputHelper.h"
struct Dir;
class NeuralNetwork;
class Joueur
{
public:
	Joueur();
	~Joueur();
	void CreateNetwork();
	void Reset(NeuralNetwork* reseau);
	int update();
	void update_();
	void move(int output);
	Pos m_pos;
	int m_reward;
	int m_previous_reward;
	int m_previous_stage;
	NeuralNetwork* m_reseau;
	InputHelper* inputHelper;
	Dir moves[11];
private:
};