
class Joueur;
class Reseau;
class Input;
class GenericNeurone;
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
	void random_network_charac(int* pn_inputs, int* pn_layers, int* pn_hidden_neurone, Reseau* parent);
	float* take_random_hidden_neurone_weights(int n, Reseau* parent, Reseau* best_joueur, int i, int j);
	Input* take_random_input(Reseau* parent, Reseau* best_joueur, Input** parent_inputs, Input** best_joueur_inputs,int n_hidden_neurones, Joueur* joueur, int i);
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
	int previous_miss_count;
};
