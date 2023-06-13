#include "Layer.h"
#include "utils.h"
#include "List.h"
#include <stdio.h>


Layer::~Layer()
{
	/*if(weights)
		delete[] weights;
	if(biases)
		delete[] biases;
	if(activations)
		delete[] activations;
	if (weightedInputs)
		delete[] weightedInputs;*/
}

Layer::Layer() : repeat_factor(0), n_nodesIn(0), n_nodesOut(0)
{
	
}

Layer::Layer(Layer* layer) : repeat_factor(layer->repeat_factor), n_nodesIn(layer->n_nodesIn), n_nodesOut(layer->n_nodesOut)
{
	printf("%d\n", layer->weights.size());
	for (int i = 0; i < n_nodesOut; i++)
	{
		biases.emplace_back(layer->biases[i]);
	}
	int size = 0;
	if (repeat_factor < n_nodesIn )
		int size = int(((n_nodesIn - 2) * repeat_factor) / (n_nodesIn - 2) + 2) * n_nodesOut;
	else
		int size = n_nodesOut * n_nodesIn;
	for (int i = 0; i < size; i++) {
		weights.emplace_back(layer->weights[i]);
	}
	weightedInputs.assign(layer->n_nodesOut, 0);
	activations.assign(layer->n_nodesOut, 0);
}
Layer::Layer(int nodesIn, int nodesOut) : n_nodesIn(nodesIn), n_nodesOut(nodesOut), repeat_factor(nodesIn+1)
{
	biases.assign(nodesOut, 0);
	weights.assign(nodesIn*nodesOut, 0);
	weightedInputs.assign(n_nodesOut, 0);
	activations.assign(n_nodesOut, 0);
	mutation();
}
Layer::Layer(int nodesIn, int nodesOut, int repeat_factor_par) : n_nodesIn(nodesIn), n_nodesOut(nodesOut), repeat_factor(repeat_factor_par)
{
	biases.assign(nodesOut, 0);
	weights.assign(int(((n_nodesIn - 2) * repeat_factor) / (n_nodesIn - 2) + 2) * nodesOut, 0);//assume this was double
	weightedInputs.assign(n_nodesOut, 0);
	activations.assign(n_nodesOut, 0);
	mutation();
}

std::vector<double> Layer::CalculateOutputs(std::vector<double> inputs)
{
	if (n_nodesIn == 12002) {
		return CalculateOutputsLayer0(inputs);
	}
	else {
		return CalculateOutputsLayerX(inputs);
	}
}

std::vector<double> Layer::CalculateOutputsLayerX(std::vector<double> inputs)
{
	/*std::vector<double> weightedInputs_ = weightedInputs;
	std::vector<double> weights_ = weights;
	std::vector<double> biases_ = biases;*/
	int a = 0;
	for (int nodeOut = 0; nodeOut < n_nodesOut; nodeOut++) {
		weightedInputs[nodeOut] = biases[nodeOut];
		for (size_t nodeIn = 0; nodeIn < weights.size(); nodeIn+=n_nodesOut) {
			//printf("%f ", inputs[nodeIn]);
			weightedInputs[nodeOut] += inputs[a++] * weights[nodeIn + nodeOut];
		}
		a = 0;
		activations[nodeOut] = ActivationFunction(weightedInputs[nodeOut]);
	}
	return activations;
}

std::vector<double> Layer::CalculateOutputsLayer0(std::vector<double> inputs)
{
	/*std::vector<double> weightedInputs_ = weightedInputs;
	std::vector<double> weights_ = weights;
	std::vector<double> biases_ = biases;*/
	for (int nodeOut = 0; nodeOut < n_nodesOut; nodeOut++) {
		weightedInputs[nodeOut] = biases[nodeOut];
		for (int nodeIn = 0; nodeIn < n_nodesIn; nodeIn++) {
			if (inputs[nodeIn] == -10000) {
				nodeIn += 6;
				continue;
			}
			if (nodeIn >= n_nodesIn - 2) {
				weightedInputs[nodeOut] += inputs[nodeIn] * weights[repeat_factor - 1 + n_nodesIn - nodeIn];
			}
			else {
				weightedInputs[nodeOut] += inputs[nodeIn] * weights[(nodeIn % repeat_factor) * n_nodesOut + nodeOut];

			}
		}
		activations[nodeOut] = ActivationFunction(weightedInputs[nodeOut]);
	}
	return activations;
}
void Layer::mutation()
{
	for (int nodeOut = 0; nodeOut < n_nodesOut; nodeOut++) {
		for (int nodeIn = 0; nodeIn < n_nodesIn; nodeIn++) {
			if (nodeIn >= n_nodesIn - 2) {
				weights[repeat_factor - 1 + n_nodesIn - nodeIn] += random_float() * 0.005;//non-reduced expr: n_nodesIn-2+(n_nodesIn-nodeIn)
			}
			else
				weights[(nodeIn % repeat_factor) * n_nodesOut + nodeOut] += random_float()*0.005;
		}
		biases[nodeOut] += random_float()*0.005;
	}
}

//double Layer::NodeCost(double outputActivation, double expectedOutput) {
//	double error = outputActivation - expectedOutput;
//	return error * error;
//}
//void Layer::UpdateGradients(double nodeValues[])
//{
//	for (int nodeOut = 0; nodeOut < n_nodesOut; nodeOut++) {
//		for (int nodeIn = 0; nodeIn < n_nodesIn; nodeIn++) {
//			double derivaticeCostWrtWeight = m_inputs[nodeIn] * nodeValues[nodeOut];
//			costGradientW[nodeIn][nodeOut] += derivaticeCostWrtWeight;
//		}
//		double derivativeCostWrtBias = nodeValues[nodeOut];
//		costGradientB[nodeOut] += derivativeCostWrtBias;
//	}
//}
//double Layer::NodeCostDerivative(double outputActivation, double expectedOutput) {
//
//	return 2 * (outputActivation - expectedOutput);
//}
//double* Layer::CalculateOutputLayerNodeValues(double expectedOutputs[], int expectedOutputs_length)
//{
//	double* nodeValues = new double[expectedOutputs_length];
//	for (int i = 0; i < expectedOutputs_length; i++) {
//		double costDerivative = NodeCostDerivative(activations[i], expectedOutputs[i]);
//		double activationDerivative = ActivationFunctionDerivative(activations[i]);
//		nodeValues[i] = activationDerivative * costDerivative;
//	}
//	return nodeValues;
//}
//double* Layer::CalculateHiddenLayerNodeValues(Layer oldlayer, double oldNodeValues[], int n_oldNodeValues_length)
//{
//	double* newNodeValues = new double[n_nodesOut];
//	for (int newNodeIndex = 0; newNodeIndex < n_nodesOut; newNodeIndex++) {
//		double newNodeValue = 0;
//		for(int oldNodeIndex = 0; oldNodeIndex < n_oldNodeValues_length; oldNodeIndex++){
//			double weightedInputDerivative = oldlayer.weights[newNodeIndex* n_oldNodeValues_length +oldNodeIndex];
//			newNodeValue += weightedInputDerivative * oldNodeValues[oldNodeIndex];
//		}
//		newNodeValue += ActivationFunctionDerivative(weightedInputs[newNodeIndex]);
//		newNodeValues[newNodeIndex] = newNodeValue;
//	}
//	delete[] oldNodeValues;
//	return newNodeValues;
//}
//void Layer::ApplyGradients(double learnRate, int n_datapoints) {
//	for (int nodeOut = 0; nodeOut < n_nodesOut; nodeOut++) {
//		biases[nodeOut] -= costGradientB[nodeOut]/200 * learnRate;
//		for (int nodeIn = 0; nodeIn < n_nodesIn; nodeIn++) {
//			weights[nodeIn* n_nodesOut +nodeOut] -= costGradientW[nodeIn][nodeOut]/200 * learnRate;
//		}
//	}
//}
//void Layer::ClearGradients() {
//	for (int nodeOut = 0; nodeOut < n_nodesOut; nodeOut++) {
//		for (int nodeIn = 0; nodeIn < n_nodesIn; nodeIn++) {
//			costGradientW[nodeIn][nodeOut] = 0;
//		}
//		costGradientB[nodeOut] = 0;
//	}
//}