#pragma once
#include <vector>

class NeuralNetwork;
class Window;
struct Node;
struct Pos;
class Drawer {
public:
	Drawer();
	Drawer(NeuralNetwork* preseau, Window* window);
	~Drawer();
	void DrawNetwork(NeuralNetwork* preseau);
	void DrawNetwork(Window* window);
	void DrawNetwork(Window* window, NeuralNetwork* preseau);
	void DrawNetwork();
	void DrawConnections(std::vector<Node> nodes, Pos pos, size_t id);
	void Apply();
	//void DrawPlayer();
	//void DrawBullets();
	//void DrawEnemies();
	//void DrawAll(List* funcs); // actually for later usage (maybe);
private:
	NeuralNetwork* m_preseau;
	Window* m_window;
	size_t NbOutputs;
};