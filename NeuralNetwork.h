#pragma once
#include "Layer.h"
#include "utils.h"

class Layer;
class NeuralNetwork
{
public:
	~NeuralNetwork();
	NeuralNetwork(int layerSizes[], int layerSizes_length);
	std::vector<double> CalculateOutputs(std::vector<double> inputs);
	int Classify(std::vector<double> inputs);
	void mutationHiddenLayer();
	bool getLayerNodesIn(int LayerToMutate);
	void AddNeurone_weights(int random_number);
	void DeleteNeurone_weights(int random_number);
	void AddNeurone_biases(int random_number);
	void DeleteNeurone_biases(int random_number);
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