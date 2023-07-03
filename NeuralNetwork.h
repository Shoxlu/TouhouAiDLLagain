#pragma once
#include <vector>
#include "utils.h"

class Layer;
#define TYPEDEFS
#define SENSOR 0
#define HIDDEN 1
#define OUTPUT 2

struct Node {
	size_t id;
	size_t type;
	double value;
};

struct Connection {
	size_t Inid;
	size_t OutId;
	double weight;
	bool state;//To tell if the connection has to be used (because overlapping connection are disabled)
	size_t InnovId; //actually i don't know how it's used;
};



class NeuralNetwork
{
public:
	~NeuralNetwork();
	NeuralNetwork(size_t inputs, size_t outputs);
	//Function to process the inputs and give outputs, 1 arguments: inputs returns an array of double
	std::vector<double> CalculateOutputs(std::vector<double> inputs);
	//Function to process an output from an input given a connection (to be use in the one before)
	double CalculateOutputConnect(size_t id);
	//Function to cross over, takes two NeuralNetwork pointers as arguments and sets itself with the child

	//Variables
	std::vector<Node> nodes;
	std::vector<double> m_inputs;
	std::vector<Connection> connections;
	size_t NbNodes;
	size_t NbConnect;
	size_t NbInputs;
	size_t NbOutputs;
	std::vector<size_t*> nodesPairs;
	size_t SpecieParentId;
private:
};