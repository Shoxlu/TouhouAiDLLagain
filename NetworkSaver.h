#pragma once
#include "CSVSaver.h"
#include "NeuralNetwork.h"
#include <string>

class NetworkSaver {
public:
	NetworkSaver();
	void SaveNetwork(NeuralNetwork* reseau, string filename);
	NeuralNetwork* GetNetwork(string filename);
	std::vector<Node> getNodes(size_t NbNodes);
	std::vector<Connection> getConnections(int n_nodesOut);
private:
	CSVSaver CSV;
};