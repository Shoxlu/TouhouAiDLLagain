
#include <d3d9.h>
#include "List.h"
class Joueur;
class Reseau;
template class List<Reseau*>;
class GenerationJoueur {
public:
	GenerationJoueur(int n_systemes);
	void first_generation();
	void new_generation();
	Joueur** get_joueurs();
	void update();
	void kill_joueur(int j);
	void fin_generation();
	void execute_command_list();
	void delete_generation();
	int get_n_systemes();
	void change_n_systemes(int nombre);
	int get_n_generation();
	int joueur_mort();

private:
	int m_n_systemes;
	const int m_n_all_systemes;
	int m_n_generation;
	int m_best_reward;
	int joueur_actuel;
	int best_joueur_id;
	Joueur** m_joueur;
	Joueur* m_best_joueur;
	List<Reseau*>* commandList;
	int previous_miss_count;
};
