using namespace std;
#include <Windows.h>
class Joueur;

void ResetGame();
class GenerationHandler {
public:
	GenerationHandler(int n_systemes);
	~GenerationHandler();
	void newGeneration();
	int update();
	void update_();
	bool joueurMort();
	int* getBestJoueurs();
	void SortBestJoueurs();
	int* getBestRewards();
	void SaveNetworks();

	int m_n_systemes;
	const int m_n_all_systemes;
	int m_n_generation;
	int* m_rewards;//array
	int* best_joueur_ids; //array
	int n_best_joueurs;
	int joueur_actuel;
	Joueur** m_joueurs;
	bool isPlaying;
private:
	int previous_miss_count;
};
