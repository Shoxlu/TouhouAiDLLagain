

#include "utils.h"

class Reseau;
struct Dir;
class Joueur
{
public:
	Joueur();
	~Joueur();
	void Reset_joueur(Reseau* reseau);
	void move(Dir direction);
	void set_pos(double x, double y);
	void set_reward(int i);

	void update();
	int get_reward();
	Reseau* get_reseau();
	Pos get_pos();
	

private:
	Pos m_pos;
	int m_reward;
	int m_previous_reward;
	int m_previous_stage;
	Reseau* m_reseau;

};