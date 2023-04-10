
#include "List.h"
class Joueur;
class Reseau;
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
	void change_n_systemes(int nombre);
	Reseau* Create_mixed_reseau(Joueur* joueur);
	void Get_parent_charac();
	bool joueur_mort();
	GenericNeurone** input_into_layer(Input** inputs, int n_inputs);
	int get_n_generation();
	int get_n_systemes();
	int get_joueur_actuel();
private:
	int m_n_systemes;
	const int m_n_all_systemes;
	int m_n_generation;
	int m_best_reward;
	int joueur_actuel;
	int best_joueur_id;
	Joueur** m_joueur;
	Joueur* m_best_joueur;
	List commandList;
	int previous_miss_count;
};
