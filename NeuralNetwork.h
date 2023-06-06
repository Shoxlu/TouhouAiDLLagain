#pragma once
#include "Layer.h"
#include "utils.h"

class Layer;
class NeuralNetwork
{
public:
	~NeuralNetwork();
	NeuralNetwork(int layerSizes[], int layerSizes_length);
	void Reset(NeuralNetwork* reseau);
	std::vector<double> CalculateOutputs(std::vector<double> inputs);
	int Classify(std::vector<double> inputs);
	void mutationHiddenLayer();
	int getLayerNodesIn(int LayerToMutate);
	void AddNode_weights(int random_number);
	void DeleteNode_weights(int random_number, int NodeOutToDelete);
	void AddNode_biases(int random_number);
	void DeleteNode_biases(int random_number, int NodeOutToDelete);
	//double Cost(DataPoint dataPoint);
	//double CostAverage(DataPoint data[], int n_data_points);
	//void UpdateAllGradients(DataPoint dataPoint, int n_datapoints);
	//void Learn(DataPoint data[], int n_data_points, double learnRate);
	//void ApplyAllGradients(double learnRate, int n_datapoints);
	//void ClearAllGradients();
	void mutation();
	int layers_length;
	int* m_layerSizes;
	Layer* layers;
private:
};