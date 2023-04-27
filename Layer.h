#pragma once
class Layer
{
public:
	Layer(int nodes_In, int nodes_Out);
	Layer();
	double* CalculateOutputs(double inputs[]);
	void recreateWeights();
	void mutation();
	void UpdateGradients(double nodeValues[]);
	void ApplyGradients(double learnRate, int n_datapoints);
	void ClearGradients();
	double NodeCost(double outputActivation, double expectedOutput);
	double NodeCostDerivative(double outputActivation, double expectedOutput);
	double* CalculateOutputLayerNodeValues(double expectedOutputs[], int expectedOutputs_length);
	double* CalculateHiddenLayerNodeValues(Layer oldlayer, double oldNodeValues[], int n_oldNodeValues_length);
	int n_nodesIn;
	int n_nodesOut;
	float** weights;
	double** costGradientW;
	double* biases;
	double* costGradientB;
	double* weightedInputs;
	double* activations;
	double* m_inputs;
private:

};