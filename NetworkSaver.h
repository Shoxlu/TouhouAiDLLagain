#pragma once
#include "CSVSaver.h"
#include <string>
using namespace std;
class NeuralNetwork;
class Layer;
class NetworkSaver {
public:
	NetworkSaver();
	~NetworkSaver();
	void SaveNetwork(NeuralNetwork* reseau, string filename);
	void SaveLayer(Layer* layer, size_t id);
	NeuralNetwork* GetNetwork(string filename);
	std::vector<double> getWeights(int id, int n_nodesIn, int n_nodesOut);
	std::vector<double> getBiases(int id, int n_nodesOut);
private:
	CSVSaver CSV;
};