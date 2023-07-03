#include "Drawer.h"
#include "NeuralNetwork.h"
#include "Window.h"
#include <stdio.h>
extern int NINPUTSPBULLET;
extern int actual_output;
extern int INPUTS_MAX;

Drawer::Drawer(): m_preseau(nullptr), m_window(nullptr) {
	//Default constructor
}

Drawer::Drawer(NeuralNetwork* preseau, Window* window) : m_preseau(preseau), m_window(window) {
	//Set up constructor
}

Drawer::~Drawer()
{
    if (m_window)
        delete m_window;
}

void Drawer::DrawNetwork() {
    NeuralNetwork* preseau = m_preseau;
    if (!preseau || !m_window) {
        //printf("Missing window or network \n");
        return;
    }
    std::vector<Connection> connections = preseau->connections;
    std::vector<Node> nodes = preseau->nodes;
    size_t NbNodes = preseau->NbNodes;
    size_t NbOutputs = preseau->NbOutputs;
    double x = -600;
    double y = 200;
    Color color = { 0, 0, 0 };
    Window* window = m_window;
    for (int i = 0; i < INPUTS_MAX; i++) {
        color = { 1, 1, 1 };
        window->draw_circle(Pos{ x, y }, 12.5, color);
        y += 25;
    }
    y = 200;
    for (size_t i = INPUTS_MAX; i < INPUTS_MAX+NbOutputs; i++) {
        color = { 0, 0.5+(nodes[i].value > 0.5 ? 0.5: 0), 0};
        window->draw_circle(Pos{ 0, y }, 12.5, color);
        y += 25;
    }
    y = 200;
    for(size_t i = 0; i < connections.size(); i++){

        size_t in = connections[i].Inid;
        size_t out = connections[i].OutId;
        Pos finalPos = Pos{0, 0};
        Pos pos = Pos{0, 0};
        if (nodes[out].type == OUTPUT) {
            finalPos = Pos{ 0, y + (out-INPUTS_MAX) * 25 };
        }else {
            finalPos = Pos{ x +50+  50 * int((out - INPUTS_MAX - NbOutputs+1) / 2) , y };
            color = { 0.5, 0.5,0.5 };
            window->draw_circle(finalPos, 12.5, color);
        }
        if (nodes[in].type == HIDDEN) {
            pos = Pos{ x + 50 +50 * int((in- INPUTS_MAX - NbOutputs+1) /2) , y};
        }
        else {
            pos = Pos{ x, y + in * 25 };
        }

        if (connections[i].state == false) {
            color = { 0.1, 0.1, 0.1 };
        }
        else if (connections[i].weight > 0) {
            color = { 1, 0, 0 };
        }
        else {
            color = { 0, 0, 1 };
        }
        window->draw_line(pos, finalPos, color);
    }

}

void Drawer::DrawNetwork(NeuralNetwork* preseau) {
	m_preseau = preseau;
	DrawNetwork();
}

void Drawer::DrawNetwork(Window* window) {
    m_window = window;
    DrawNetwork();
}

void Drawer::DrawNetwork(Window* window, NeuralNetwork* preseau) {
    m_window = window;
    m_preseau = preseau;
    DrawNetwork();
}

void Drawer::Apply() {
    if (!m_window) {
        printf("no Window ? \n");
        return;
    } 
    m_window->update();
}