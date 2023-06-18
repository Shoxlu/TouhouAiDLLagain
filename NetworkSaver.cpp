#include "NetworkSaver.h"
#include "NeuralNetwork.h"

NetworkSaver::NetworkSaver()
{
}

NetworkSaver::~NetworkSaver()
{
}

void NetworkSaver::SaveNetwork(NeuralNetwork* reseau, string filename)
{
	
	CSVFile* newFile = CSV.create(filename);
	size_t layers_length = reseau->layers_length;
	Layer* layers = reseau->layers;
	for (size_t i = 0; i < layers_length; i++) {
		SaveLayer(&layers[i], i);
	}

	char layerSize[16];
	for (size_t i = 0; i < layers_length + 1; i++) {
		snprintf(layerSize, 16, "%d", reseau->m_layerSizes[i]);
		CSV.write(layerSize, "Layers_Sizes", i);
	}
	CSV.Close();
}

void NetworkSaver::SaveLayer(Layer* layer, size_t id)
{
	size_t n_nodesIn = 0;
	if(id == 0){
		n_nodesIn = 8;
	}
	else {
		n_nodesIn = layer->n_nodesIn;
	}
	size_t n_nodesOut = layer->n_nodesOut;
	char key[128];
	char value[16];

	for (size_t nodeOut = 0; nodeOut < n_nodesOut; nodeOut++) {
		snprintf(key, 128, "Layer%d_Biases", id);
		snprintf(value, 16, "%f", layer->biases[nodeOut]);
		CSV.write(value, key, nodeOut);
		for (size_t nodeIn = 0; nodeIn < n_nodesIn; nodeIn++) {
			snprintf(key, 128, "Layer%d_NodeOut%d", id, nodeOut);
			snprintf(value, 16, "%f", layer->weights[nodeIn * n_nodesOut + nodeOut]);
			CSV.write(value, key, nodeIn);
		}
	}
	
}

NeuralNetwork* NetworkSaver::GetNetwork(string filename)
{
	CSVFile* file = CSV.read(filename);
	if (file == nullptr) {
		printf("Failed to open file");
		return nullptr;
	}
	
	//get layers sizes
	std::vector<int> layerSizes;
	char buffer[16];
	size_t length = 0;
	for (size_t i = 0; buffer != "NULL" && i < file->n_lines; i++) {
		layerSizes.emplace_back((int)buffer);
		length = i;
	}

	//get each layer with GetLayer
	Layer* layers = new Layer[length - 1];
	layers[0] = std::move(Layer(layerSizes[0], layerSizes[1], 6));
	for (size_t i = 1; i < length - 1; i++) {
		layers[i] = std::move(Layer(layerSizes[i], layerSizes[i + 1]));	
		layers[i].weights = getWeights(i, layerSizes[i], layerSizes[i + 1]);
		layers[i].biases = getBiases(i, layerSizes[i + 1]);
	}


	NeuralNetwork* reseau = new NeuralNetwork();
	reseau->layers = layers;
	reseau->layers_length = length - 1;
	reseau->m_layerSizes = new int[length];
	for (size_t i = 0; i < length; i++) {
		reseau->m_layerSizes[i] = layerSizes[i];
	}
	return reseau;
}

std::vector<double> NetworkSaver::getWeights(int id, int n_nodesIn, int n_nodesOut)
{
	std::vector<double> weights;
	char key[128];
	for (size_t nodeOut = 0; nodeOut < n_nodesOut; nodeOut++) {
		for (size_t nodeIn = 0; nodeIn < n_nodesIn; nodeIn++) {
			snprintf(key, 128, "Layer%d_NodeOut%d", id, nodeOut);
			weights.emplace_back((double)CSV.read(key, nodeIn));
		}
	}
	return weights;
}
std::vector<double> NetworkSaver::getBiases(int id, int n_nodesOut)
{
	std::vector<double> biases;
	char key[128];
	for (size_t nodeOut = 0; nodeOut < n_nodesOut; nodeOut++) {
		snprintf(key, 128, "Layer%d_NodeOut%d", id, nodeOut);
		biases.emplace_back(CSV.read(key, nodeOut));
	}
	return biases;
}