#include "NetworkSaver.h"
#include "NeuralNetwork.h"
NetworkSaver::NetworkSaver()
{
}

void NetworkSaver::SaveNetwork(NeuralNetwork* reseau, int32_t reward, string filename)
{
	
	CSVFile* newFile = CSV.create(filename);
	char buffer[512];
	char buffer1[512];
	for (size_t i = 0; i < reseau->connections.size(); i++) {
		sprintf_s(buffer,sizeof(buffer), "Connection%d", i);
		sprintf_s(buffer1,sizeof(buffer1), "%f", reseau->connections[i].weight);
		CSV.write(buffer1, buffer, 0);
		sprintf_s(buffer1,sizeof(buffer1), "%d", reseau->connections[i].Inid);
		CSV.write(buffer1, buffer, 1);
		sprintf_s(buffer1,sizeof(buffer1), "%d", reseau->connections[i].OutId);
		CSV.write(buffer1, buffer, 2);
		sprintf_s(buffer1,sizeof(buffer1), "%d", reseau->connections[i].state);
		CSV.write(buffer1, buffer, 3);
		sprintf_s(buffer1,sizeof(buffer1), "%d", reseau->connections[i].InnovId);
		CSV.write(buffer1, buffer, 4);
	}
	for (size_t i = 0; i < reseau->nodes.size(); i++) {
		sprintf_s(buffer1,sizeof(buffer1), "%d", reseau->nodes[i].type);
		CSV.write(buffer1, "Node", reseau->nodes[i].id);
	}
	sprintf_s(buffer1,sizeof(buffer1), "%d", reseau->connections.size());
	CSV.write(buffer1, "Other_Infos", 0);
	sprintf_s(buffer1,sizeof(buffer1), "%d", reseau->nodes.size());
	CSV.write(buffer1, "Other_Infos", 1);
	sprintf_s(buffer1, sizeof(buffer1), "%d", reward);
	CSV.write(buffer1, "Other_Infos", 2);
	CSV.Apply();
	CSV.Close();
}


NeuralNetwork* NetworkSaver::GetNetwork(string filename, int32_t* reward)
{
	CSVFile* file = CSV.read(filename);
	if (file == nullptr) {
		printf("Failed to open file");
		return nullptr;
	}
	
	NeuralNetwork* reseau = new NeuralNetwork(INPUTS_MAX, OUTPUTS_MAX);
	string test = CSV.read("Other_Infos", 0);
	reseau->connections = getConnections(stoi(CSV.read("Other_Infos", 0)));
	reseau->nodes = getNodes(stoi(CSV.read("Other_Infos", 1)));
	*reward = stoi(CSV.read("Other_Infos", 2));
	CSV.Close();
	return reseau;
}

vector<Node> NetworkSaver::getNodes(size_t NbNodes)
{
	std::vector<Node> nodes;
	for (size_t n_node = 0; n_node < NbNodes; n_node++) {
		Node node = Node();
		node.id = n_node;
		node.type = stoi(CSV.read("Node", n_node)),
		nodes.emplace_back(node);
	}
	return nodes;
}

vector<Connection> NetworkSaver::getConnections(size_t NBConnect)
{
	vector<Connection> connections;
	char buffer[512];
	for (size_t i = 0; i < NBConnect; i++) {
		Connection connection = Connection();
		sprintf_s(buffer,sizeof(buffer), "Connection%d", i);
		connection.weight = stod(CSV.read(buffer, 0));
		connection.Inid = stoi(CSV.read(buffer, 1));
		connection.OutId = stoi(CSV.read(buffer, 2));
		connection.state = stoi(CSV.read(buffer, 3));
		connection.InnovId = stoi(CSV.read(buffer, 4));
		connections.emplace_back(connection);
	}
	return connections;
} 