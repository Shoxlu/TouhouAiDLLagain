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
	void Reset();
	void update();
	void move(std::vector<double> outputs);
	Pos m_pos;
	int m_reward;
	int m_previous_reward;
	int m_previous_stage;
	NeuralNetwork* m_reseau;
	InputHelper* inputHelper;
	Dir moves[6];
private:
};