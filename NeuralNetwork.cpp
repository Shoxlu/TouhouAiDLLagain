#ifndef TYPEDEF
#define SENSOR 0
#define HIDDEN 1
#define OUTPUT 2
#endif // !TYPEDEF
#include "NeuralNetwork.h"
extern size_t NbInnov; //still don't know but it will be helpful
extern int INPUTS_MAX;


NeuralNetwork::NeuralNetwork(size_t inputs, size_t outputs): SpecieParentId(0), m_inputs(), connections(), nodes(), NbConnect(0), NbInputs(inputs), NbOutputs(outputs), NbNodes(inputs+outputs) {
	nodes.assign(NbNodes, Node());
	for (size_t i = 0; i < nodes.size(); i++) {
		nodes[i].id = i;
		if (i < NbInputs)
			nodes[i].type = SENSOR;
		else
			nodes[i].type = OUTPUT;
	}
	for (size_t i = 0; i < NbInputs; i++) {
		for (size_t j = NbInputs; j < NbInputs+NbOutputs; j++) {
			size_t* pair = new size_t[2];
			pair[0] = i;
			pair[1] = j;
			nodesPairs.emplace_back(pair);
		}
	}
}
std::vector<double> NeuralNetwork::CalculateOutputs(std::vector<double> inputs)
{
	std::vector<double> outputs;
	for (size_t i = 0; i < NbNodes; i++) {
		nodes[i].value = 0;
	}
	for (size_t i = 0; i < NbInputs; i++) {
		nodes[i].value = inputs[i];
	}
	for (size_t i = 0; i < connections.size(); i++) {
		if (connections[i].state) {
			//double avantTraitement = nodes[connections[i].OutId].value
			nodes[connections[i].OutId].value += nodes[connections[i].Inid].value * connections[i].weight;

			/*-- on ""allume"" le lien si la connexion a fait une modif
			if avantTraitement ~= unReseau.lesNeurones[unReseau.lesConnexions[i].sortie].valeur then
				unReseau.lesConnexions[i].allume = true
			else
				unReseau.lesConnexions[i].allume = false
			end*/
		}
	}
	for (size_t i = 0; i < NbOutputs; i++) {
		outputs.emplace_back(ActivationFunction(nodes[INPUTS_MAX+i].value));
	}
	return outputs;
}




NeuralNetwork::~NeuralNetwork() {

}