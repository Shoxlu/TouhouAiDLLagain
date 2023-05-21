#include "Layer.h"
#include "utils.h"
#include "List.h"
#include <stdio.h>


Layer::~Layer()
{
	if(weights)
		delete[] weights;
	if(biases)
		delete[] biases;
	if(activations)
		delete[] activations;
	if (weightedInputs)
		delete[] weightedInputs;
}

Layer::Layer() : repeat_factor(0), n_nodesIn(0), n_nodesOut(0), weights(nullptr), biases(nullptr), activations(nullptr),weightedInputs(nullptr)
{
	
}
Layer::Layer(int nodesIn, int nodesOut) : n_nodesIn(nodesIn), n_nodesOut(nodesOut), repeat_factor(nodesIn+1)
{
	biases = new double[nodesOut];
	weights = new float[nodesIn*nodesOut];//assume this was double
	weightedInputs = new double[n_nodesOut];
	activations = new double[n_nodesOut];
	for (int nodeOut = 0; nodeOut < nodesOut; nodeOut++) {
		biases[nodeOut] = 0;
		for (int nodeIn = 0; nodeIn < nodesIn; nodeIn++) {
			weights[nodeIn* nodesOut + nodeOut] = 0;
		}
	}
	mutation();
}
Layer::Layer(int nodesIn, int nodesOut, int repeat_factor_par) : n_nodesIn(nodesIn), n_nodesOut(nodesOut), repeat_factor(repeat_factor_par)
{
	biases = new double[nodesOut];
	weights = new float[int(nodesIn/ repeat_factor_par) * nodesOut];//assume this was double
	weightedInputs = new double[n_nodesOut];
	activations = new double[n_nodesOut];
	for (int nodeOut = 0; nodeOut < nodesOut; nodeOut++) {
		biases[nodeOut] = 0;
		for (int nodeIn = 0; nodeIn < nodesIn; nodeIn++) {
			weights[nodeIn % repeat_factor_par * nodesOut + nodeOut] = 0;
		}
	}
	mutation();
}

double* Layer::CalculateOutputs(double inputs[])
{
	for (int nodeOut = 0; nodeOut < n_nodesOut; nodeOut++) {
		weightedInputs[nodeOut] = biases[nodeOut];
		for (int nodeIn = 0; nodeIn < n_nodesIn; nodeIn++) {
			//printf("%f ", inputs[nodeIn]);
			if (inputs[nodeIn] == -10000) {
				nodeIn += 6;
				continue;
			}
			weightedInputs[nodeOut] += inputs[nodeIn] * weights[nodeIn % repeat_factor * n_nodesOut + nodeOut];
		}
		activations[nodeOut] = ActivationFunction(weightedInputs[nodeOut]);
	}
	//printf("\n");
	return activations;
}
void Layer::mutation()
{
	for (int nodeOut = 0; nodeOut < n_nodesOut; nodeOut++) {
		for (int nodeIn = 0; nodeIn < n_nodesIn; nodeIn++) {
			weights[nodeIn % repeat_factor * n_nodesOut + nodeOut] += random_float()*0.01;
		}
		biases[nodeOut] += random_float()*0.01;
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