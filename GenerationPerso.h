#pragma once
#include <Windows.h>
#include <vector>
#include "Joueur.h"
#include "NeuralNetwork.h"
#include "NetworkSaver.h"
#include "utils.h"

void ResetGame_wrap();
class GenerationHandler {
public:
	GenerationHandler(int32_t n_systemes);
	~GenerationHandler();
	Joueur** newGeneration(Joueur** newPopulation);
	void update();

	std::vector<Specie> SortSpeciesByReward(std::vector<Specie> Species);
	int32_t getBestReward(Joueur** population, NeuralNetwork** bestNetwork);
	void AddNode(NeuralNetwork* reseau, size_t ConnectId);
	void AddConnection(size_t in, size_t out, NeuralNetwork* reseau);
	void DeleteConnection(size_t index, NeuralNetwork* reseau);
	void DoMutation(NeuralNetwork* reseau);
	size_t CheckForExistingConnection(size_t in, size_t out);
	void SaveNetworks(Joueur** population);
	bool LoadNetworks(Joueur*** newPopulation);
	std::vector<Specie> sortPopulationBySpecies(Joueur** population);
	size_t getNetworkDiff(NeuralNetwork* reseau1, NeuralNetwork* reseau2, double* diffPoids);
	double getScore(NeuralNetwork* networkToTest, NeuralNetwork* networkToCompare);
	int32_t findBestNetworkInOldPop(NeuralNetwork** bestNetwork);
	void ResetGame();

	size_t m_n_systemes;
	int32_t m_generationId;
	size_t joueur_actuel;
	Joueur** populationActuelle;
	bool isPlaying;
	size_t NbInnov = 0;
	Joueur** m_previousBestGen;
private:
};
