#include "NeuralNetwork.h"
#include "InputHelper.h"
#include "utils.h"
#include "List.h"
#include <algorithm>


NeuralNetwork::NeuralNetwork(int layerSizes[], int layerSizes_length)
{
	m_layerSizes = new int[layerSizes_length];
	copy_array(layerSizes_length, m_layerSizes, layerSizes);
	layers = new Layer[layerSizes_length - 1];
	layers[0] = std::move(Layer(layerSizes[0], layerSizes[1], 6));
	for (int i = 1; i < layerSizes_length - 1; i++) { 
		layers[i] = std::move(Layer(layerSizes[i], layerSizes[i + 1]));
	}
	layers_length = layerSizes_length-1;
}
NeuralNetwork::~NeuralNetwork()
{
	if (layers) {
		delete[] layers;
	}
	if (m_layerSizes)
		delete[] m_layerSizes;
}


std::vector<double> NeuralNetwork::CalculateOutputs(std::vector<double> inputs)
{
	//layers[0].n_nodesIn = n_inputs;
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
	for (int i = 0; i < layers_length; i++) {

		layers[i].mutation();
	}
}

void NeuralNetwork::mutationHiddenLayer()
{
	if (randint(0, 100) < 10) {//change that to duplicate 1st layer weights from one set of input (one bullet)
		int LayerToMutate = randint(1, layers_length-1);//layers_length -> out of range(layers), layers_length-1 -> output layer , 0->inputs
		m_layerSizes[LayerToMutate] += 1;
		AddNode_weights(LayerToMutate-1);
		AddNode_biases(LayerToMutate-1);
		
	}
	if (randint(0, 100) < 10) {
		
		for (int i = 0; i < layers_length + 2; i++) {
			if (m_layerSizes[i] > 1)
				continue;
			if(i == layers_length + 1)
			{
				return;
			}
		}
		int LayerToMutate = randint(1, layers_length - 1);
		while (m_layerSizes[LayerToMutate] <= 1) {
			LayerToMutate = randint(1, layers_length - 1);
		}
		m_layerSizes[LayerToMutate] -= 1;
		DeleteNode_weights(LayerToMutate-1);
		DeleteNode_biases(LayerToMutate-1);
	}

}
bool NeuralNetwork::getLayerNodesIn(int LayerToMutate) {
	int repeat_factor = layers[LayerToMutate].repeat_factor;
	int n_nodesOut = layers[LayerToMutate].n_nodesOut;
	if (LayerToMutate == 0) {
		return ((layers[LayerToMutate].n_nodesIn - 2) * repeat_factor) / (layers[LayerToMutate].n_nodesIn - 2) + 2;
	}
	return layers[LayerToMutate].n_nodesIn;
}
void NeuralNetwork::AddNode_weights(int LayerToMutate)
{
	auto& layer = layers[LayerToMutate];
	auto& nextLayer = layers[LayerToMutate + 1];
	
	++layer.n_nodesOut;
	int n_nodesOut = layer.n_nodesOut;
	int n_nodesOut1 = nextLayer.n_nodesOut;
	size_t n_nodesIn = getLayerNodesIn(LayerToMutate);

	auto begin = layer.weights.begin();
	for (size_t nodeIn = 0; nodeIn < n_nodesIn; nodeIn++) {
		layer.weights.insert(begin + nodeIn * n_nodesOut + n_nodesOut - 1, 0);
	}

	if (layers_length > 1 && LayerToMutate + 1 < layers_length) {
		++nextLayer.n_nodesIn;
		size_t new_weight_count = nextLayer.n_nodesIn * n_nodesOut1;
		size_t prev_weight_count = (nextLayer.n_nodesIn - 1) * n_nodesOut1;
		begin = nextLayer.weights.begin();
		for (size_t i = prev_weight_count; i < new_weight_count; ++i) {
			nextLayer.weights.insert(begin+i, random_float());
		}
		nextLayer.weightedInputs.emplace_back(0);
		nextLayer.activations.emplace_back(0);
	}
	layer.weightedInputs.emplace_back(0);
	layer.activations.emplace_back(0);
}

void NeuralNetwork::DeleteNode_weights(int LayerToMutate)
{
	auto& layer = layers[LayerToMutate];
	auto& nextLayer = layers[LayerToMutate + 1];

	--layer.n_nodesOut;
	int n_nodesOut = layer.n_nodesOut;
	int n_nodesOut1 = nextLayer.n_nodesOut;
	size_t n_nodesIn = getLayerNodesIn(LayerToMutate);

	auto begin = layer.weights.begin();
	int NodeOutToDelete = randint(0, m_layerSizes[LayerToMutate+1]-1);
	for (size_t nodeIn = 0; nodeIn < layers[LayerToMutate].n_nodesIn; nodeIn++) {
		layer.weights.erase(begin + nodeIn * n_nodesOut + NodeOutToDelete);
	}

	if (layers_length > 1 && LayerToMutate + 1 < layers_length) {
		--nextLayer.n_nodesIn;
		for (size_t nodeOut = 0; nodeOut < n_nodesOut1; nodeOut++)
		{
			layer.weights.erase(begin + NodeOutToDelete * n_nodesOut1 + nodeOut);
		}
		nextLayer.weightedInputs.pop_back();
		nextLayer.activations.pop_back();
	}
	layer.weightedInputs.pop_back();
	layer.activations.pop_back();
}

void NeuralNetwork::AddNode_biases(int LayerToMutate) {
	auto& layer = layers[LayerToMutate];
	auto begin = layer.biases.begin();
	layers[LayerToMutate].biases.insert(begin+layers[LayerToMutate].n_nodesOut-1, random_float());
}

void NeuralNetwork::DeleteNode_biases(int LayerToMutate) {
	auto& layer = layers[LayerToMutate];
	auto begin = layer.biases.begin();
	layers[LayerToMutate].biases.erase(begin + layers[LayerToMutate].n_nodesOut - 1);
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
//}