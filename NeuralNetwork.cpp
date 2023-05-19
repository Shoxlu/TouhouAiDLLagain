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
	if (randint(0, 100) < 2) {//change that to duplicate 1st layer weights from one set of input (one bullet)
		int random_number = randint(0, layers_length-2);//layers_length -> out of range(layers), layers_length-1 -> output layer , 0->inputs
		m_layerSizes[random_number] += 1;
		AddNeurone_weights(random_number);
		AddNeurone_biases(random_number);
	}
	if (randint(0, 100) < 2) {//change that to duplicate 1st layer weights from one set of input (one bullet)
		
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
	int repeat_factor = layers[random_number].repeat_factor;
	for (int nodeIn = 0; nodeIn < layers[random_number].n_nodesIn; nodeIn++) {
		for (int nodeOut = 0; nodeOut < n_nodesOut - 1; nodeOut++)
		{
			new_weights[nodeIn % repeat_factor * n_nodesOut +nodeOut] = actual_weights[nodeIn * n_nodesOut + nodeOut];
		}
		new_weights[nodeIn % repeat_factor * n_nodesOut + n_nodesOut - 1] = random_float();//ajoute un nodeOut pour chaque nodeIn
	}
	
	for (int nodeIn = 0; nodeIn < n_nodesOut - 1; nodeIn++) {
		for (int nodeOut = 0; nodeOut < n_nodesOut1; nodeOut++)
		{
			new_weights1[nodeIn * n_nodesOut1 + nodeOut] = actual_weights1[nodeIn * n_nodesOut1 + nodeOut];
		}
	}
	for (int nodeOut = 0; nodeOut < n_nodesOut1; nodeOut++)
	{
		new_weights1[(n_nodesOut - 1) * n_nodesOut + nodeOut] = random_float();//créé un weight dans chaque nodeOut pour ce nodeIn
	}
	if (actual_weights)
		delete[] actual_weights;
	else
		printf("There was an error on delete[] actual_weights ? Not initialised? check potential bad allocations.(AddNeurone) \n");
	if(actual_weights1)
		delete[] actual_weights1;
	else
		printf("There was an error on delete[] actual_weights1 ? Not initialised? check potential bad allocations.(AddNeurone) \n");
	
	layers[random_number].weights = new_weights;
	layers[random_number + 1].weights = new_weights1;
}
void NeuralNetwork::DeleteNeurone_weights(int random_number)
{
	
	layers[random_number].n_nodesOut = m_layerSizes[random_number];
	layers[random_number + 1].n_nodesIn = m_layerSizes[random_number];
	int repeat_factor = layers[random_number].repeat_factor;
	int n_nodesOut = m_layerSizes[random_number];
	int n_nodesOut1 = layers[random_number + 1].n_nodesOut;
	float* new_weights = new float[layers[random_number].n_nodesIn * n_nodesOut];
	float* new_weights1 = new float[n_nodesOut * n_nodesOut1];
	float* actual_weights = layers[random_number].weights;
	float* actual_weights1 = layers[random_number + 1].weights;
	int random_number2 = randint(0, m_layerSizes[random_number]-1);
	
	int n = 0;
	for (int nodeIn = 0; nodeIn < layers[random_number].n_nodesIn; nodeIn++) {
		for (int nodeOut = 0; nodeOut < n_nodesOut; nodeOut++)
		{
			if (random_number2 == nodeOut) {
				n = 1;
			}
			new_weights[nodeIn % repeat_factor * n_nodesOut + nodeOut] = actual_weights[nodeIn * n_nodesOut + nodeOut + n];
		}
	}
	n = 0;
	for (int nodeIn = 0; nodeIn < n_nodesOut; nodeIn++) {
		if (random_number2 == nodeIn) {
			n = 1;
		}
		for (int j = 0; j < n_nodesOut1; j++)
		{
			new_weights1[nodeIn * n_nodesOut1 + j] = actual_weights1[(nodeIn + n) * n_nodesOut1 + j];
		}
	}
	if (actual_weights)
		delete[] actual_weights;
	else
		printf("There was an error on delete[] actual_weights ? Not initialised? check potential bad allocations.(DeleteNeurone) \n");
	if (actual_weights1)
		delete[] actual_weights1;
	else
		printf("There was an error on delete[] actual_weights1 ? Not initialised? check potential bad allocations.(DeleteNeurone) \n");

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

	if (actual_biases)
		delete[] actual_biases;
	else
		printf("There was an error on delete[] actual_biases ? Not initialised? check potential bad allocations. (AddNeurone)\n");
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
	if (actual_biases)
		delete[] actual_biases;
	else
		printf("There was an error on delete[] actual_biases ? Not initialised? check potential bad allocations. (DeleteNeurone)\n");
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