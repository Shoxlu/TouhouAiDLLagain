#pragma once
#include <vector>
#include "utils.h"


class NeuralNetwork
{
public:
	NeuralNetwork(size_t inputs, size_t outputs);
	//Function to process the inputs and give outputs, 1 arguments: inputs returns an array of double
	std::vector<double> CalculateOutputs(std::vector<double> inputs);
	void CrossOver(NeuralNetwork* parent1, NeuralNetwork* parent2);
	//Variables
	std::vector<Node> nodes;
	std::vector<double> m_inputs;
	std::vector<Connection> connections;
	size_t NbInputs;
	size_t NbOutputs;
	std::vector<size_t*> nodesPairs;
	size_t SpecieParentId;
private:
};