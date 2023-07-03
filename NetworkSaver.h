#pragma once
#include "CSVSaver.h"
#include "NeuralNetwork.h"
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
	std::vector<Node> getNodes(size_t NbNodes);
	std::vector<Connection> getConnections(int n_nodesOut);
private:
	CSVSaver CSV;
};