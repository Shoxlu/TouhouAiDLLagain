#include "Drawer.h"
#include "NeuralNetwork.h"
#include "Window.h"
#include <stdio.h>
Drawer::Drawer():NbOutputs(0), m_preseau(nullptr), m_window(nullptr) {
	//Default constructor
}

Drawer::Drawer(NeuralNetwork* preseau, Window* window) : NbOutputs(0), m_preseau(preseau), m_window(window) {
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
    std::vector<Node> sorted_nodes = preseau->sorted_nodes;
    std::vector<Node> nodes = preseau->nodes;
    size_t NbNodes = preseau->nodes.size();
    NbOutputs = preseau->NbOutputs;
    double x = -600;
    double y = -400;
    Color color = { 0, 0, 0 };
    Window* window = m_window;
    //probably gonna make something better with some linked pair or something and list of positions
    for (size_t i = 0; i < nodes.size(); i++) {
        Pos pos = {};
        color = { 0, 0.5 + (nodes[i].value > 0 ? 0.5 : 0), 0 };
        switch (nodes[i].type)
        {
        case SENSOR:
            color = { 1, 1, 1 };
            pos = Pos{ x, y + 75 * i };
            break;
        case OUTPUT:
            pos = Pos{ 0,-1000+ 75.0 * i - INPUTS_MAX };
            DrawConnections(nodes, pos, i);
            break;
        case HIDDEN:
            pos = Pos{ -300, -100.0 + 400/(nodes.size()-INPUTS_MAX-NbOutputs+1)* (i-INPUTS_MAX - NbOutputs+1) };
            DrawConnections(nodes, pos, i);
            break;
        }
        window->draw_circle(pos, 12.5, color);
    }
   /* y = 200;
    for(size_t i = 0; i < connections.size(); i++){

        size_t in = connections[i].Inid;
        size_t out = connections[i].OutId;
        Pos finalPos = Pos{0, 0};
        Pos pos = Pos{0, 0};
        if (nodes[out].type == OUTPUT) {
            finalPos = Pos{ 0, y + (out-INPUTS_MAX) * 25 };
        }else {
            finalPos = Pos{ x +50+  50 * out , y };
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
    }*/

}

void Drawer::DrawConnections(std::vector<Node> nodes, Pos pos, size_t id) {
    Color color;
    for (size_t j = 0; j < nodes[id].incoming_connections.size(); j++) {
        if (nodes[id].incoming_connections[j].state == false) {
            color = { 0.1, 0.1, 0.1 };
        }
        else if (nodes[id].incoming_connections[j].weight > 0) {
            color = { 1, 0, 0 };
        }
        else {
            color = { 0, 0, 1 };
        }
        Pos pos1 = { 0, 0 };
        switch(nodes[nodes[id].incoming_connections[j].Inid].type){
        case SENSOR:
            pos1 = Pos{ -600, -400.0 + 75 * nodes[id].incoming_connections[j].Inid };
            break;
        case OUTPUT:
            pos1 = Pos{  0, 75.0 * nodes[id].incoming_connections[j].Inid - INPUTS_MAX };
            break;
        case HIDDEN:
            pos1 = Pos{ -300, -100+400.0 / (nodes.size() - INPUTS_MAX - NbOutputs + 1) * (nodes[id].incoming_connections[j].Inid - INPUTS_MAX - NbOutputs + 1) };
            break;
        }
        //add edge case for inputs to outputs connections.
        window->draw_line(pos1, pos, color);
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