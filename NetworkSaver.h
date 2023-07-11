#pragma once
#include "CSVSaver.h"
#include "NeuralNetwork.h"
#include <string>

class NetworkSaver {
public:
	NetworkSaver();
	void SaveNetwork(NeuralNetwork* reseau, int32_t reward, string filename);
	NeuralNetwork* GetNetwork(string filename, int32_t* reward);
	std::vector<Node> getNodes(size_t NbNodes);
	std::vector<Connection> getConnections(size_t n_nodesOut);
private:
	CSVSaver CSV;
};