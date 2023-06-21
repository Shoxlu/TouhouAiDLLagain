#include "NeuralNetwork.h"
#include "InputHelper.h"
#include "utils.h"
#include "List.h"
#include <algorithm>


NeuralNetwork::NeuralNetwork(): m_layerSizes(), layers(nullptr), layers_length(0) {

}

NeuralNetwork::NeuralNetwork(std::vector<int> layerSizes)
{
	m_layerSizes = layerSizes;
	size_t layerSizes_length = layerSizes.size();
	layers = new Layer[layerSizes_length - 1];
	layers[0] = std::move(Layer(layerSizes[0], layerSizes[1], 6));
	for (int i = 1; i < layerSizes_length - 1; i++) { 
		layers[i] = std::move(Layer(layerSizes[i], layerSizes[i + 1]));
	}
	layers_length = layerSizes_length-1;
}

void NeuralNetwork::Reset(NeuralNetwork* reseau) {
	std::vector<int> layerSizes = reseau->m_layerSizes;
	int layerSizes_length = (reseau->layers_length + 1);
	m_layerSizes = layerSizes;
	layers = new Layer[layerSizes_length - 1];
	for (int i = 0; i < layerSizes_length - 1; i++) {
		layers[i] = Layer(&reseau->layers[i]);
	}
	layers_length = layerSizes_length - 1;
}

NeuralNetwork::~NeuralNetwork()
{
	if (layers) {
		delete[] layers;
	}
}


std::vector<double> NeuralNetwork::CalculateOutputs(std::vector<double> inputs)
{
	for (int i = 0; i < layers_length; i++) {
		inputs = layers[i].CalculateOutputs(inputs);
	}
	return inputs;
}

int NeuralNetwork::Classify(std::vector<double> inputs) {
	std::vector<double> outputs = CalculateOutputs(inputs);
	int max_i = GetMaximumIndex(outputs, m_layerSizes[layers_length]);
	return max_i;
}


void NeuralNetwork::mutation()
{
	mutationHiddenLayer();
	mutationLayers();
	for (int i = 0; i < layers_length; i++) {

		layers[i].mutation();
	}
}


void NeuralNetwork::AddLayer() {
	//Add a layer:
	//Resize m_layersizes
	int layersToAdd = randint(1, layers_length);
	auto begin = m_layerSizes.begin();
	m_layerSizes.insert(begin + layersToAdd, 1);
	//Increment layers length
	++layers_length;
	//Reorganize "layers" to implement a new Layer
	Layer* newLayers = new Layer[layers_length];
	for (int i = 0; i < layers_length; i++) {
		newLayers[i] = Layer(&layers[i]);
	}
	delete[] layers;
	layers = newLayers;
	layers[layersToAdd - 1] = std::move(Layer(m_layerSizes[layersToAdd - 1], m_layerSizes[layersToAdd]));
	layers[layersToAdd] = std::move(Layer(m_layerSizes[layersToAdd], m_layerSizes[layersToAdd + 1]));
}

void NeuralNetwork::RemoveLayer() {
	//Remove a layer:
	//if layers length > 1:
	if (layers_length > 1)
	{
		int layersToDelete = randint(1, layers_length);
		//Resize m_layersizes
		auto begin = m_layerSizes.begin();
		m_layerSizes.erase(begin + layersToDelete);

		//Decrement layers length
		--layers_length;
		//Reorganize "layers" to delete a Layer
		Layer* newLayers = new Layer[layers_length];
		for (int i = 0; i < layers_length; i++) {
			if (i == layersToDelete) {
				continue;
			}
			else {
				newLayers[i] = Layer(&layers[i]);
			}

		}
		delete[] layers;
		layers = newLayers;
		layers[layersToDelete - 1] = std::move(Layer(m_layerSizes[layersToDelete - 1], m_layerSizes[layersToDelete]));
	}
}

void NeuralNetwork::mutationLayers() {
	if (randint(0, 100) < 1) {
		AddLayer();
	}
	if (randint(0, 100) < 1) {
		RemoveLayer();
	}
}


void NeuralNetwork::mutationHiddenLayer()
{
	if (randint(0, 100) < 2) {//change that to duplicate 1st layer weights from one set of input (one bullet)
		int LayerToMutate = randint(1, layers_length-1);//layers_length -> out of range(layers), layers_length-1 -> output layer , 0->inputs
		m_layerSizes[LayerToMutate] += 1;
		AddNode_weights(LayerToMutate-1);
		AddNode_biases(LayerToMutate-1);
		
	}
	if (randint(0, 100) < 2) {
		
		for (int i = 1; i < layers_length; i++) {
			if (m_layerSizes[i] > 1)
				continue;
			if(i == layers_length - 1)
			{
				return;
			}
		}
		int LayerToMutate = randint(1, layers_length - 1);
		while (m_layerSizes[LayerToMutate] <= 1) {
			LayerToMutate = randint(1, layers_length - 1);
		}
		m_layerSizes[LayerToMutate] -= 1;
		int NodeOutToDelete = randint(0, m_layerSizes[LayerToMutate] - 1);
		DeleteNode_weights(LayerToMutate-1, NodeOutToDelete);
		DeleteNode_biases(LayerToMutate-1, NodeOutToDelete);
	}

}
int NeuralNetwork::getLayerNodesIn(int LayerToMutate) {
	auto& layer = layers[LayerToMutate];
	if (LayerToMutate == 0) {
		return ((layer.n_nodesIn - 2) * layer.repeat_factor) / (layer.n_nodesIn - 2) + 2;
	}
	return layer.n_nodesIn;
}
void NeuralNetwork::AddNode_weights(int LayerToMutate)
{
	auto& layer = layers[LayerToMutate];
	auto& nextLayer = layers[LayerToMutate + 1];
	
	++layer.n_nodesOut;
	int n_nodesOut = layer.n_nodesOut;
	int n_NextNodesOut = nextLayer.n_nodesOut;
	auto begin = layer.weights.begin();
	int n_nodesIn = getLayerNodesIn(LayerToMutate);
	for (size_t i = n_nodesOut-1; i <= layer.weights.size(); i+= n_nodesOut) {
		begin = layer.weights.begin();
		layer.weights.insert(begin+i, random_float()*0.005);
	}

	if (layers_length > 1 && LayerToMutate + 1 < layers_length) {
		++nextLayer.n_nodesIn;
		for (int i = 0; i < n_NextNodesOut; ++i) {
			nextLayer.weights.emplace_back(random_float()*0.005);
		}
		nextLayer.repeat_factor++;
		nextLayer.weightedInputs.emplace_back(0);
		nextLayer.activations.emplace_back(0);
	}
	layer.weightedInputs.emplace_back(0);
	layer.activations.emplace_back(0);
}

void NeuralNetwork::DeleteNode_weights(int LayerToMutate, int NodeOutToDelete)
{
	auto& layer = layers[LayerToMutate];
	auto& nextLayer = layers[LayerToMutate + 1];

	--layer.n_nodesOut;
	int n_nodesOut = layer.n_nodesOut;
	size_t n_nodesIn = getLayerNodesIn(LayerToMutate);
	auto begin = layer.weights.begin();
	for (size_t pos = NodeOutToDelete; pos < layer.weights.size(); pos += n_nodesOut) {
		begin = layer.weights.begin();
		layer.weights.erase(begin+pos);
	}
	if (layers_length > 1 && LayerToMutate + 1 < layers_length) {
		--nextLayer.n_nodesIn;
		auto next_pos = nextLayer.weights.begin() + NodeOutToDelete * nextLayer.n_nodesOut;
		nextLayer.weights.erase(next_pos, next_pos+ nextLayer.n_nodesOut);
		nextLayer.repeat_factor--;
		nextLayer.weightedInputs.pop_back();
		nextLayer.activations.pop_back();
	}
	layer.weightedInputs.pop_back();
	layer.activations.pop_back();
}

void NeuralNetwork::AddNode_biases(int LayerToMutate) {
	auto& layer = layers[LayerToMutate];
	layers[LayerToMutate].biases.emplace_back(random_float()* 0.005);
}

void NeuralNetwork::DeleteNode_biases(int LayerToMutate, int NodeOutToDelete) {
	auto& layer = layers[LayerToMutate];
	layers[LayerToMutate].biases.erase(layer.biases.begin()+ NodeOutToDelete);
}

//double NeuralNetwork::Cost(DataPoint dataPoint) {
//	double* outputs = CalculateOutputs(dataPoint.inputs, 2);
//	Layer outputLayer = layers[layers_length];
//	double cost = 0;
//	for (int nodeOut = 0; nodeOut < m_layerSizes[layers_length]; nodeOut++)
//	{
//		cost += outputLayer.NodeCost(outputs[nodeOut], dataPoint.expectedOutputs[nodeOut]);
//	}
//	return cost;
//}
//double NeuralNetwork::CostAverage(DataPoint data[], int n_datapoints) {
//	double totalCost = 0;
//	for (int i = 0; i < n_datapoints; i++)
//	{
//		totalCost += Cost(data[i]);
//	}
//	return totalCost / n_datapoints;
//}
//
//void NeuralNetwork::Learn(DataPoint data[], int n_datapoints, double learnRate)
//{
//	for (int dataIndex= 0; dataIndex < n_datapoints; dataIndex++){
//		UpdateAllGradients(data[dataIndex], n_datapoints);
//	}
//
//	ApplyAllGradients(learnRate / n_datapoints, n_datapoints);
//
//	ClearAllGradients();
//}
//void NeuralNetwork::UpdateAllGradients(DataPoint dataPoint, int n_datapoints)
//{
//	CalculateOutputs(dataPoint.inputs, 2);
//	Layer outputLayer = layers[layers_length - 1];
//	double* nodeValues = outputLayer.CalculateOutputLayerNodeValues(dataPoint.expectedOutputs, 2);
//	outputLayer.UpdateGradients(nodeValues);
//	for (int hiddenLayerIndex = layers_length - 2; hiddenLayerIndex >= 0; hiddenLayerIndex--)
//	{
//		Layer hiddenLayer = layers[hiddenLayerIndex];
//		nodeValues = hiddenLayer.CalculateHiddenLayerNodeValues(layers[hiddenLayerIndex + 1], 
//			nodeValues, 
//			layers[hiddenLayerIndex + 1].n_nodesOut);
//		hiddenLayer.UpdateGradients(nodeValues);
//	}
//	delete[] nodeValues;
//}
//void NeuralNetwork::ApplyAllGradients(double learnRate, int n_datapoints)
//{
//	for (int layer_index = 0; layer_index < layers_length; layer_index++) {
//		layers[layer_index].ApplyGradients(learnRate, n_datapoints);
//	}
//}
//void NeuralNetwork::ClearAllGradients() {
//	for (int layer_index = 0; layer_index < layers_length; layer_index++) {
//		layers[layer_index].ClearGradients();
//	}
//}r