using namespace std;
#include <Windows.h>
class Joueur;

void ResetGame();
class GenerationJoueur {
public:
	GenerationJoueur(int n_systemes);
	~GenerationJoueur();
	void newGeneration();
	int update();
	void update_();
	bool joueurMort();
	int* getBestJoueurs();
	int* getBestRewards();
	int m_n_systemes;
	const int m_n_all_systemes;
	int m_n_generation;

	int* m_rewards;//array
	int* best_joueur_ids; //array
	int n_best_joueurs;
	int joueur_actuel;
	Joueur* m_joueurs;
	bool isPlaying;
private:
	int previous_miss_count;
};
