#pragma once
#include <vector>
class Layer
{
public:
	Layer(int nodes_In, int nodes_Out);
	Layer(Layer* Layer);
	Layer(int nodes_In, int nodes_Out, int repeat_factor_par);
	Layer();
	~Layer();
	Layer(Layer&& src) noexcept
	{
		n_nodesIn = src.n_nodesIn;
		n_nodesOut = src.n_nodesOut;

		weights = src.weights;
		//src.weights = nullptr;

		biases = src.biases;
		//src.biases = nullptr;

		weightedInputs = src.weightedInputs;
		//src.weightedInputs = nullptr;

		activations = src.activations;
		//src.activations = nullptr;
		repeat_factor = src.repeat_factor;

	}
	Layer& operator=(Layer&& src) noexcept{
		n_nodesIn = src.n_nodesIn;
		n_nodesOut = src.n_nodesOut;
		repeat_factor = src.repeat_factor;
		weights = src.weights;
		//src.weights = nullptr;

		biases = src.biases;
		//src.biases = nullptr;

		weightedInputs = src.weightedInputs;
		//src.weightedInputs = nullptr;

		activations = src.activations;
		//src.activations = nullptr;
		return *this;
	}
	std::vector<double> CalculateOutputs(std::vector<double> inputs);
	std::vector<double> CalculateOutputsLayerX(std::vector<double> inputs);
	std::vector<double> CalculateOutputsLayer0(std::vector<double> inputs);
	void recreateWeights();
	void mutation();
	/*void UpdateGradients(double nodeValues[]);
	void ApplyGradients(double learnRate, int n_datapoints);
	void ClearGradients();
	double NodeCost(double outputActivation, double expectedOutput);
	double NodeCostDerivative(double outputActivation, double expectedOutput);
	double* CalculateOutputLayerNodeValues(double expectedOutputs[], int expectedOutputs_length);
	double* CalculateHiddenLayerNodeValues(Layer oldlayer, double oldNodeValues[], int n_oldNodeValues_length);*/
	int n_nodesIn;
	int n_nodesOut;
	std::vector<double> weights;
	std::vector<double> biases;
	std::vector<double> weightedInputs;
	std::vector<double> activations;
	int repeat_factor;
private:

};