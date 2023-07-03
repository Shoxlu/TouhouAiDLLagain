using namespace std;
#include <Windows.h>
#include "NeuralNetwork.h"
class Joueur;

struct Specie {
	std::vector<Joueur*> networks;
	int avgReward = 0;
	int maxReward = 0;
	int totalReward = 0;
	size_t nbChildren = 0;
};

void ResetGame_wrap();
class GenerationHandler {
public:
	GenerationHandler(int n_systemes);
	~GenerationHandler();
	Joueur** newGeneration(Joueur** newPopulation);
	void update();
	bool joueurMort();
	std::vector<Specie> SortSpeciesByReward(std::vector<Specie> Species);
	int getBestReward(Joueur** population, NeuralNetwork** bestNetwork);
	void AddNode(NeuralNetwork* reseau, size_t ConnectId);
	void AddConnection(size_t in, size_t out, NeuralNetwork* reseau);
	void DoMutation(NeuralNetwork* reseau);
	void CrossOver(NeuralNetwork* parent1, NeuralNetwork* parent2, NeuralNetwork* crossSpring);
	size_t CheckForExistingConnection(size_t in, size_t out);
	void SaveNetworks(Joueur** population);
	bool LoadNetworks(Joueur*** newPopulation);
	std::vector<Specie> sortPopulationBySpecies(Joueur** population);
	size_t getNetworkDiff(NeuralNetwork* reseau1, NeuralNetwork* reseau2, double* diffPoids);
	double getScore(NeuralNetwork* networkToTest, NeuralNetwork* networkToCompare);
	size_t findBestNetworkInOldPop(NeuralNetwork** bestNetwork);
	void ResetGame();

	size_t m_n_systemes;
	int m_generationId;
	size_t joueur_actuel;
	Joueur** populationActuelle;
	bool isPlaying;
	size_t NbInnov = 0; //still don't know but it will be helpful
	size_t GlNbNodes = 0;
	Joueur** m_previousBestGen;

private:
};
