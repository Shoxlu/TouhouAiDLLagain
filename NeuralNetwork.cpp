
#include "NeuralNetwork.h"
#include <cassert>




NeuralNetwork::NeuralNetwork(size_t inputs, size_t outputs): SpecieParentId(0), m_inputs(), connections(), nodes(), NbInputs(inputs), NbOutputs(outputs) {
	nodes.assign(inputs + outputs, Node());
	for (size_t i = 0; i < nodes.size(); i++) {
		nodes[i].id = i;
		if (i < NbInputs)
			nodes[i].type = SENSOR;
		else
			nodes[i].type = OUTPUT;
	}
}


NeuralNetwork::~NeuralNetwork() {
    for (size_t i = 0; i < nodesPairs.size(); i++) {
        delete[] nodesPairs[i];
    }
    nodesPairs.~vector();
}

std::vector<double> NeuralNetwork::CalculateOutputs(std::vector<double> inputs)
{
	std::vector<double> outputs;
	for (size_t i = 0; i < nodes.size(); i++) {
        sorted_nodes[i].value = 0;
	}
    for (size_t i = 0; i < nodes.size(); i++) {
        if (sorted_nodes[i].type == SENSOR) {
            sorted_nodes[i].value = inputs[i];
            nodes[i].value = inputs[i];
        }
        else
        {
            double weightedInput = 0;
            for (size_t j = 0; j < sorted_nodes[i].incoming_connections.size(); j++) {
                if (sorted_nodes[i].incoming_connections[j].state)
                    weightedInput += nodes[sorted_nodes[i].incoming_connections[j].Inid].value * sorted_nodes[i].incoming_connections[j].weight;
            }
            sorted_nodes[i].value = ActivationFunction(weightedInput);
            nodes[sorted_nodes[i].id].value = sorted_nodes[i].value;
        }
        if (sorted_nodes[i].type == OUTPUT)
            outputs.emplace_back(sorted_nodes[i].value);
    }
	/*for (size_t i = 0; i < connections.size(); i++) {
		if (connections[i].state) {
			double avantTraitement = nodes[connections[i].OutId].value
			nodes[connections[i].OutId].value += nodes[connections[i].Inid].value * connections[i].weight;

			-- on ""allume"" le lien si la connexion a fait une modif
			if avantTraitement ~= unReseau.lesNeurones[unReseau.lesConnexions[i].sortie].valeur then
				unReseau.lesConnexions[i].allume = true
			else
				unReseau.lesConnexions[i].allume = false
			end
		}
	}*/

	return outputs;
}


bool NeuralNetwork::checkForCompatibleNodes(Connection connection) {
    bool flagIn = false;
    bool flagOut = false;
    for (size_t i = 0; i < nodes.size(); i++) {
        if (connection.Inid == nodes[i].id)
            flagIn = true;
        if (connection.OutId == nodes[i].id)
            flagOut = true;
    }
    return flagIn && flagOut;
}


bool NeuralNetwork::CheckForExistingConnectionInNetwork(size_t i, size_t j) {
    for (size_t k = 0; k < connections.size(); k++) {
        if (connections[k].Inid == i && connections[k].OutId == j) {
            return true;
        }
    }
    return false;
}



void NeuralNetwork::CrossOver(NeuralNetwork* parent1, NeuralNetwork* parent2, size_t reward1, size_t reward2) {
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
    NeuralNetwork* bestNetwork;
    NeuralNetwork* worseNetwork;
    if (reward1 > reward2)
    {
        bestNetwork = parent1;
        worseNetwork = parent2;
    }
    else {
        worseNetwork = parent1;
        bestNetwork = parent2;
    }
    nodes = bestNetwork->nodes;
    std::vector<Connection> newConnections;
    newConnections = bestNetwork->connections;
    size_t MaxInnovId_best = 0;
    if(newConnections.size() > 0)
         MaxInnovId_best = getMaxInnovId(newConnections);
    for (size_t i = 0; i < worseNetwork->connections.size(); i++) {
        size_t InnovId_worse = worseNetwork->connections[i].InnovId;
        for (size_t j = 0; j < bestNetwork->connections.size(); j++) {
            //if best's InnovId and worse's InnovId are equal
            size_t InnovId_best = bestNetwork->connections[j].InnovId;
            if (InnovId_best == InnovId_worse) {
                if (random_bool()) {
                    newConnections[j].weight = worseNetwork->connections[i].weight;
                }
                break;
            }
            assert(MaxInnovId_best >= InnovId_best);
        }
        if (InnovId_worse > MaxInnovId_best)//if not: check if worse's Innovid is bigger than max best's Innovid
        {//--if yes:it's an excessing connection, add it to newConnections if possible
            if (checkForCompatibleNodes(worseNetwork->connections[i]))
                newConnections.emplace_back(worseNetwork->connections[i]);
        }
    }
    
    connections = newConnections;
}

void NeuralNetwork::DoNodesPairs() {
    std::vector<size_t*> nodesPairs_temp;
    for (size_t i = 0; i < nodes.size(); i++) {
        for (size_t j = 0; j < nodes.size(); j++) {
            if (i == j || nodes[i].type == OUTPUT || nodes[j].type == SENSOR || CheckForExistingConnectionInNetwork(i, j) || CheckForExistingConnectionInNetwork(j, i))
                continue;
            size_t* pair = new size_t[2];
            pair[0] = i;
            pair[1] = j;
            nodesPairs_temp.emplace_back(pair);
        }
    }
    for (size_t i = 0; i < nodesPairs.size(); i++) {
        delete[] nodesPairs[i];
    }
    nodesPairs = nodesPairs_temp;
}


void NeuralNetwork::sortIncomingConnections()
{
    if (connections.size() == 0)
        return;
    for (size_t i = NbInputs; i < nodes.size(); i++)
    {
        nodes[i].incoming_connections.clear();
        for (size_t j = 0; j < connections.size(); j++) {
            if (connections[j].OutId == i) {
                nodes[i].incoming_connections.emplace_back(connections[j]);
            }
        }
    }
}

void NeuralNetwork::topologicalSortNodes() {
    sorted_nodes.clear();  // Clear the sorted_nodes list before starting the sort
    for (size_t i = 0; i < nodes.size(); i++) {
        nodes[i].already_visited = false;
    }
    // Iterate through each node and visit it if it hasn't been already
    for (size_t i = 0; i < nodes.size(); i++) {
        if (!nodes[i].already_visited) {
            visitNode(i);
        }
    }
}

void NeuralNetwork::visitNode(size_t id) {
    // Set the already_visited flag to true for the visited node
    nodes[id].already_visited = true;

    // Check for incoming_connections size, if it's greater than 0
    if (nodes[id].incoming_connections.size() > 0) {
        // Iterate through the incoming connections
        for (size_t i = 0; i < nodes[id].incoming_connections.size(); i++) {
            size_t inId = nodes[id].incoming_connections[i].Inid;
            if (!nodes[inId].already_visited && nodes[id].incoming_connections[i].state) {
                visitNode(inId);  // Recursively visit the connected node if not already visited   
            }
        }
    }
    sorted_nodes.emplace_back(nodes[id]);
      // Add the visited node to the sorted list (at the end)
}