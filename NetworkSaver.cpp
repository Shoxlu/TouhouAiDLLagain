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
	string value = CSV.read("Layers_Sizes", 0);
	string null = "NULL";
	size_t length = 0;
	for (size_t i = 0; value != null && i < file->n_lines; i++) {
		layerSizes.emplace_back(stoi(value));
		length = i+1;
		value = CSV.read("Layers_Sizes", i+1);
	}
	if (length < 1){
		return nullptr;
	}
	//get each layer with GetLayer
	Layer* layers = new Layer[length - 1];
	layers[0] = std::move(Layer(layerSizes[0], layerSizes[1], 6));
	layers[0].weights = getWeights(0, 8, layerSizes[1]);
	layers[0].biases = getBiases(0, layerSizes[1]);
	for (size_t i = 1; i < length - 1; i++) {
		layers[i] = std::move(Layer(layerSizes[i], layerSizes[i + 1]));	
		layers[i].weights = getWeights(i, layerSizes[i], layerSizes[i + 1]);
		layers[i].biases = getBiases(i, layerSizes[i + 1]);
	}


	NeuralNetwork* reseau = new NeuralNetwork();
	reseau->layers = layers;
	reseau->layers_length = length - 1;
	reseau->m_layerSizes.clear();
	for (size_t i = 0; i < length; i++) {
		reseau->m_layerSizes.emplace_back(layerSizes[i]);
	}
	CSV.Close();
	return reseau;
}

vector<double> NetworkSaver::getWeights(int id, int n_nodesIn, int n_nodesOut)
{
	std::vector<double> weights;
	char key[18];
	for (int nodeOut = 0; nodeOut < n_nodesOut; nodeOut++) {
		for (int nodeIn = 0; nodeIn < n_nodesIn; nodeIn++) {
			snprintf(key, 18, "Layer%d_NodeOut%d", id, nodeOut);
			weights.emplace_back(stod(CSV.read(key, nodeIn)));
		}
	}
	return weights;
}

vector<double> NetworkSaver::getBiases(int id, int n_nodesOut)
{
	vector<double> biases;
	char key[18];
	for (int nodeOut = 0; nodeOut < n_nodesOut; nodeOut++) {
		snprintf(key, 18, "Layer%d_Biases", id);
		biases.emplace_back(stod(CSV.read(key, nodeOut)));
	}
	return biases;
}