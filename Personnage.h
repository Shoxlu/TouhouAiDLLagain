
#include <d2d1.h>

class Reseau;
struct Dir;
class Joueur
{
public:
	Joueur();
	void Reset_joueur(Reseau* reseau);
	~Joueur();
	//Joueur(Reseau* reseau);
	void move(Dir direction);
	void update();
	int get_reward();
	Reseau* get_reseau();
	float get_x() const;
	float get_y() const;
	void create_Reseau();
	void set_pos(double x, double y);

	void set_reward(int i);

private:
	float m_x;
	float m_y;
	int m_reward;
	int m_previous_reward;
	int m_previous_stage;
	Reseau* m_reseau;

};