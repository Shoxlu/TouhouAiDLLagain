#include "NeuralNetwork.h"
#include "InputHelper.h"
#include "utils.h"
#include <algorithm>


NeuralNetwork::NeuralNetwork(int layerSizes[], int layerSizes_length)
{
	m_layerSizes = new int[layerSizes_length];
	copy_array(layerSizes_length, m_layerSizes, layerSizes);
	layers = new Layer[layerSizes_length - 1];
	for (int i = 0; i < layerSizes_length - 1; i++) { 
		layers[i] = std::move(Layer(layerSizes[i], layerSizes[i + 1]));
	}
	layers_length = layerSizes_length-1;
}
NeuralNetwork::~NeuralNetwork()
{
	if (layers != nullptr) {
		delete[] layers;
	}
	if (m_layerSizes != nullptr)
		delete[] m_layerSizes;
}


double* NeuralNetwork::CalculateOutputs(double inputs[], int n_inputs)
{
	//layers[0].n_nodesIn = n_inputs;
	for (int i = 0; i < layers_length; i++) {
		inputs = layers[i].CalculateOutputs(inputs);
	}
	return inputs;
}

int NeuralNetwork::Classify(double inputs[], int n_inputs) {
	double* outputs = CalculateOutputs(inputs, n_inputs);
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
	if (randint(0, 100) < 2) {
		int random_number = randint(1, layers_length-2);//layers_length -> out of range(layers), layers_length-1 -> output layer , 1->inputs
		m_layerSizes[random_number] += 1;
		AddNeurone_weights(random_number);
		AddNeurone_biases(random_number);
	}
	if (randint(0, 100) < 2) {
		
		for (int i = 0; i < layers_length + 2 && m_layerSizes[i] <= 1; i++) {
			if (i == layers_length + 1)
			{
				return;
			}
		}
		int random_number = randint(1, layers_length - 2);
		while (m_layerSizes[random_number] <= 1) {
			random_number = randint(1, layers_length - 2);
		}
		m_layerSizes[random_number] -= 1;
		DeleteNeurone_weights(random_number);
		DeleteNeurone_biases(random_number);
	}

}

void NeuralNetwork::AddNeurone_weights(int random_number)
{
	layers[random_number].n_nodesOut = m_layerSizes[random_number];
	layers[random_number + 1].n_nodesIn = m_layerSizes[random_number];
	int n_nodesOut = m_layerSizes[random_number];
	int n_nodesOut1 = layers[random_number + 1].n_nodesOut;
	float* new_weights = new float[layers[random_number].n_nodesIn * n_nodesOut];
	float* new_weights1 = new float[n_nodesOut * n_nodesOut1];
	float* actual_weights = layers[random_number].weights;
	float* actual_weights1 = layers[random_number + 1].weights;
	
	for (int i = 0; i < layers[random_number].n_nodesIn; i++) {
		for (int j = 0; j < n_nodesOut - 1; j++)
		{
			new_weights[i* n_nodesOut +j] = actual_weights[i * n_nodesOut + j];
		}
		new_weights[i * n_nodesOut + n_nodesOut - 1] = random_float();//ajoute un nodeOut pour chaque nodeIn
	}
	
	for (int i = 0; i < n_nodesOut1 - 1; i++) {
		for (int j = 0; j < n_nodesOut1; j++)
		{
			new_weights1[i * n_nodesOut1 + j] = actual_weights1[i * n_nodesOut1 + j];
		}
	}
	for (int j = 0; j < n_nodesOut1; j++)
	{
		new_weights1[(n_nodesOut - 1) * n_nodesOut1 + j] = random_float();//créé un weight dans chaque nodeOut pour ce nodeIn
	}
	delete[] actual_weights;
	delete[] actual_weights1;
	layers[random_number].weights = new_weights;
	layers[random_number + 1].weights = new_weights1;
}
void NeuralNetwork::DeleteNeurone_weights(int random_number)
{
	
	layers[random_number].n_nodesOut = m_layerSizes[random_number];
	layers[random_number + 1].n_nodesIn = m_layerSizes[random_number];
	int n_nodesOut = m_layerSizes[random_number];
	int n_nodesOut1 = layers[random_number + 1].n_nodesOut;
	float* new_weights = new float[layers[random_number].n_nodesIn * n_nodesOut];
	float* new_weights1 = new float[n_nodesOut * n_nodesOut1];
	float* actual_weights = layers[random_number].weights;
	float* actual_weights1 = layers[random_number + 1].weights;
	int random_number2 = randint(0, m_layerSizes[random_number]-1);
	
	int n = 0;
	for (int i = 0; i < layers[random_number].n_nodesIn; i++) {
		for (int j = 0; j < n_nodesOut; j++)
		{
			if (random_number2 == j) {
				n = 1;
			}
			new_weights[i * n_nodesOut + j] = actual_weights[i * n_nodesOut + j + n];
			printf("new_weights[%d][%d] = %f\n", i, j, actual_weights[i * n_nodesOut + j + n]);
		}
	}
	n = 0;
	for (int i = 0; i < n_nodesOut; i++) {
		if (random_number2 == i) {
			n = 1;
		}
		for (int j = 0; j < n_nodesOut1; j++)
		{
			new_weights1[i * n_nodesOut1 + j] = actual_weights1[(i + n) * n_nodesOut1 + j];
			printf("new_weights1[%d][%d] = %f\n", i, j, actual_weights1[(i + n) * n_nodesOut1 + j]);
		}
	}
	delete[] actual_weights;
	delete[] actual_weights1;
	layers[random_number].weights = new_weights;
	layers[random_number + 1].weights = new_weights1;
}

void NeuralNetwork::AddNeurone_biases(int random_number) {
	layers[random_number].n_nodesOut = m_layerSizes[random_number];
	double* new_biases = new double[layers[random_number].n_nodesOut];
	double* actual_biases = layers[random_number].biases;

	for (int i = 0; i < layers[random_number].n_nodesOut-1; i++) {
		new_biases[i] = actual_biases[i];
	}
	new_biases[layers[random_number].n_nodesOut] = random_float();
	delete[] actual_biases;
	layers[random_number].biases = new_biases;
}
void NeuralNetwork::DeleteNeurone_biases(int random_number) {
	layers[random_number].n_nodesOut = m_layerSizes[random_number];
	double* new_biases = new double[layers[random_number].n_nodesOut];
	double* actual_biases = layers[random_number].biases;
	int n = 0;
	int random_number2 = randint(0, m_layerSizes[random_number]-1);
	for (int i = 0; i < m_layerSizes[random_number]; i++) {
		if (random_number2 == i)
		{
			n = 1;
		}
		new_biases[i] = actual_biases[i+n];
	}
	delete[] actual_biases;
	layers[random_number].biases = new_biases;
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