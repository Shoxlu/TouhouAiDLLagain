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


double* NeuralNetwork::CalculateOutputs(double inputs[])
{
	//layers[0].n_nodesIn = n_inputs;
	for (int i = 0; i < layers_length; i++) {
		inputs = layers[i].CalculateOutputs(inputs);
	}
	return inputs;
}

int NeuralNetwork::Classify(double inputs[]) {
	double* outputs = CalculateOutputs(inputs);
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
		AddNeurone_weights(LayerToMutate-1);
		AddNeurone_biases(LayerToMutate-1);
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
		DeleteNeurone_weights(LayerToMutate-1);
		DeleteNeurone_biases(LayerToMutate-1);
	}

}
bool NeuralNetwork::newWeightsCreation(float** newWeights, int LayerToMutate) {
	int repeat_factor = layers[LayerToMutate].repeat_factor;
	int n_nodesOut = layers[LayerToMutate].n_nodesOut;
	if (LayerToMutate == 0) {
		*newWeights = new float[int(((layers[LayerToMutate].n_nodesIn - 2) * repeat_factor) / (layers[LayerToMutate].n_nodesIn - 2) + 2) * n_nodesOut];
		
	}
	else
		*newWeights = new float[layers[LayerToMutate].n_nodesIn * n_nodesOut];
	if (!newWeights) {
		printf("bad array allocation");
		return 0;
	}
	else {
		return 1;
	}
}
void NeuralNetwork::AddNeurone_weights(int LayerToMutate)
{
	layers[LayerToMutate].n_nodesOut +=1;
	
	int n_nodesOut = layers[LayerToMutate].n_nodesOut;
	int n_nodesOut1 = layers[LayerToMutate + 1].n_nodesOut;
	int repeat_factor = layers[LayerToMutate].repeat_factor;
	float* newWeights;
	if (newWeightsCreation(&newWeights, LayerToMutate) == 0) {
		return;
	}
	
	float* actualWeights = layers[LayerToMutate].weights;
	
	for (int nodeIn = 0; nodeIn < layers[LayerToMutate].n_nodesIn; nodeIn++) {
		for (int nodeOut = 0; nodeOut < n_nodesOut - 1; nodeOut++)
		{
			newWeights[nodeIn % repeat_factor * n_nodesOut +nodeOut] = actualWeights[nodeIn%repeat_factor * n_nodesOut + nodeOut];
		}
		newWeights[nodeIn % repeat_factor * n_nodesOut + n_nodesOut - 1] = random_float();//ajoute un nodeOut pour chaque nodeIn
	}
	if (layers_length > 1 && LayerToMutate + 1 < layers_length) {
		layers[LayerToMutate + 1].n_nodesIn -= 1;
		float* newWeights1 = new float[n_nodesOut * n_nodesOut1];
		float* actualWeights1 = layers[LayerToMutate + 1].weights;
		for (int nodeIn = 0; nodeIn < n_nodesOut - 1; nodeIn++) {
			for (int nodeOut = 0; nodeOut < n_nodesOut1; nodeOut++)
			{
				newWeights1[nodeIn * n_nodesOut1 + nodeOut] = actualWeights1[nodeIn * n_nodesOut1 + nodeOut];
			}
		}
			for (int nodeOut = 0; nodeOut < n_nodesOut1; nodeOut++)
			{
				newWeights1[(n_nodesOut - 1) * n_nodesOut1 + nodeOut] = random_float();//créé un weight dans chaque nodeOut pour ce nodeIn
			}

		if (actualWeights1 != nullptr)
			delete[] actualWeights1;
		else
			printf("There was an error on delete[] actual_weights1 ? Not initialised? check potential bad allocations.(DeleteNeurone) \n");
		layers[LayerToMutate + 1].weights = newWeights1;
	}
	if (actualWeights)
		delete[] actualWeights;
	else
		printf("There was an error on delete[] actual_weights ? Not initialised? check potential bad allocations.(DeleteNeurone) \n");
	layers[LayerToMutate].weights = newWeights;
}

void NeuralNetwork::DeleteNeurone_weights(int LayerToMutate)
{
	
	layers[LayerToMutate].n_nodesOut -= 1;
	int repeat_factor = layers[LayerToMutate].repeat_factor;

	int n_nodesOut = layers[LayerToMutate].n_nodesOut;
	int n_nodesOut1 = layers[LayerToMutate + 1].n_nodesOut;
	float* newWeights;
	if (newWeightsCreation(&newWeights, LayerToMutate) == 0) {
		return;
	}

	float* actualWeights = layers[LayerToMutate].weights;
	int NodeOutToDelete = randint(0, m_layerSizes[LayerToMutate+1]-1);
	
	int n = 0;
	for (int nodeIn = 0; nodeIn < layers[LayerToMutate].n_nodesIn; nodeIn++) {
		for (int nodeOut = 0; nodeOut < n_nodesOut; nodeOut++)
		{
			if (NodeOutToDelete == nodeOut) {
				n = 1;
			}
			newWeights[nodeIn % repeat_factor * n_nodesOut + nodeOut] = actualWeights[nodeIn % repeat_factor * n_nodesOut + nodeOut + n];
		}
	}
	if (layers_length > 1 && LayerToMutate + 1 < layers_length) {
		layers[LayerToMutate + 1].n_nodesIn -= 1;
		float* newWeights1 = new float[n_nodesOut * n_nodesOut1];
		float* actualWeights1 = layers[LayerToMutate + 1].weights;
		n = 0;
		for (int nodeIn = 0; nodeIn < n_nodesOut; nodeIn++) {
			if (NodeOutToDelete == nodeIn) {
				n = 1;
			}
			for (int j = 0; j < n_nodesOut1; j++)
			{
				newWeights1[nodeIn * n_nodesOut1 + j] = actualWeights1[(nodeIn + n) * n_nodesOut1 + j];
			}	
		}
		if (actualWeights1 != nullptr)
			delete[] actualWeights1;
		else
			printf("There was an error on delete[] actual_weights1 ? Not initialised? check potential bad allocations.(DeleteNeurone) \n");
		layers[LayerToMutate + 1].weights = newWeights1;
	}
	if (actualWeights)
		delete[] actualWeights;
	else
		printf("There was an error on delete[] actual_weights ? Not initialised? check potential bad allocations.(DeleteNeurone) \n");
	layers[LayerToMutate].weights = newWeights;
}

void NeuralNetwork::AddNeurone_biases(int LayerToMutate) {
	layers[LayerToMutate].n_nodesOut +=1;
	double* newBiases = new double[layers[LayerToMutate].n_nodesOut];
	double* actualBiases = layers[LayerToMutate].biases;

	for (int i = 0; i < layers[LayerToMutate].n_nodesOut-1; i++) {
		newBiases[i] = actualBiases[i];
	}
	newBiases[layers[LayerToMutate].n_nodesOut-1] = random_float();

	if (actualBiases) {
		delete[] actualBiases;
	}
	else
		printf("There was an error on delete[] actual_biases ? Not initialised? check potential bad allocations. (AddNeurone)\n");
	layers[LayerToMutate].biases = newBiases;
}

void NeuralNetwork::DeleteNeurone_biases(int LayerToMutate) {
	layers[LayerToMutate].n_nodesOut -= 1;
	double* newBiases = new double[layers[LayerToMutate].n_nodesOut];
	double* actualBiases = layers[LayerToMutate].biases;
	int n = 0;
	int NodeOutToDelete = randint(0, m_layerSizes[LayerToMutate+1]-1);
	for (int i = 0; i < layers[LayerToMutate].n_nodesOut; i++) {
		if (NodeOutToDelete == i)
		{
			n = 1;
		}
		newBiases[i] = actualBiases[i+n];
	}
	if (actualBiases)
		delete[] actualBiases;
	else
		printf("There was an error on delete[] actual_biases ? Not initialised? check potential bad allocations. (DeleteNeurone)\n");
	layers[LayerToMutate].biases = newBiases;
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