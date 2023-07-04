
#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(size_t inputs, size_t outputs): SpecieParentId(0), m_inputs(), connections(), nodes(), NbInputs(inputs), NbOutputs(outputs) {
	nodes.assign(inputs + outputs, Node());
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
	for (size_t i = 0; i < nodes.size(); i++) {
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
		outputs.emplace_back(ActivationFunction(nodes[NbInputs+i].value));
	}
	return outputs;
}



void NeuralNetwork::CrossOver(NeuralNetwork* parent1, NeuralNetwork* parent2) {
    /*size_t max_NbInnov = 0;
    if (parent1->connections.size() > 0 && parent2->connections.size() > 0) {
        max_NbInnov = max(parent1->connections.back().InnovId, parent2->connections.back().InnovId);
    }
    else if (parent2->connections.size() > 0)
        max_NbInnov = parent2->connections.back().InnovId;
    else if (parent1->connections.size() > 0)
        max_NbInnov = parent1->connections.size();
    Connection* connection = nullptr;
    Connection* connection1 = nullptr;
    for (size_t InnovId = 0; InnovId < max_NbInnov + 1; InnovId++) {
        connection = GetConnectionById(&parent1->connections, InnovId);
        connection1 = GetConnectionById(&parent2->connections, InnovId);
        if (connection && connection1) {
            //we'll assume that the crossSpring is getting parent1 connection weight in this case
            connection->state &= connection->state;
            newConnections.emplace_back(connection);
        }
        else if (connection) {
            newConnections.emplace_back(connection);
        }
        else if (connection1) {
            newConnections.emplace_back(connection1);
        }
    }*/
    std::vector<Connection> newConnections;
    newConnections = parent2->connections;
    bool found = false;
    for (size_t i = 0; i < parent1->connections.size(); i++) {
        for (size_t j = 0; j < parent2->connections.size(); j++) {
            if (parent1->connections[i].InnovId == parent2->connections[j].InnovId) {
                newConnections[j].state &= parent1->connections[i].state;
                found = true;
                break;
            }
        }
        if (!found)
            newConnections.emplace_back(parent1->connections[i]);
        found = false;
    }
    connections = newConnections;
    if (parent1->nodes.size() > parent2->nodes.size())
        nodes = parent1->nodes;
    else
        nodes = parent2->nodes;
    std::vector<size_t*> nodesPairs;
    for (size_t j = 0; j < nodes.size(); j++) {
        for (size_t i = 0; i < nodes.size(); i++) {
            if (i == j || nodes[i].type == OUTPUT || nodes[j].type == SENSOR)
                continue;
            size_t* pair = new size_t[2];
            pair[0] = i;
            pair[1] = j;
            nodesPairs.emplace_back(pair);
        }
    }
    for (size_t i = 0; i < nodesPairs.size(); i++) {
        delete[] nodesPairs[i];
    }
    nodesPairs = nodesPairs;
}