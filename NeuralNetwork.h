#pragma once
#include "utils.h"


class NeuralNetwork
{
public:
	NeuralNetwork(size_t inputs, size_t outputs);
	~NeuralNetwork();
	//Function to process the inputs and give outputs, 1 arguments: inputs returns an array of double
	std::vector<double> CalculateOutputs(std::vector<double> inputs);
	bool checkForCompatibleNodes(Connection connection);
	bool CheckForExistingConnectionInNetwork(size_t i, size_t j);
	void CrossOver(NeuralNetwork* parent1, NeuralNetwork* parent2, size_t reward1, size_t reward2);
	void DoNodesPairs();
	void sortIncomingConnections();
	void topologicalSortNodes();
	void visitNode(size_t id);
	//Variables
	std::vector<Node> sorted_nodes;
	std::vector<Node> nodes;
	std::vector<double> m_inputs;
	std::vector<Connection> connections;
	size_t NbInputs;
	size_t NbOutputs;
	std::vector<size_t*> nodesPairs;
	size_t SpecieParentId;
private:
};