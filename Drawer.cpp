#include "Drawer.h"
#include "NeuralNetwork.h"
#include "Window.h"
#include <stdio.h>

extern int actual_output;
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
    float color = 1;
    int layers_length = preseau->layers_length;
    for (int i = 0; i < layers_length; i++)
    {
        DrawLayer(i);
    }
    for (int i = 0; i < preseau->m_layerSizes[layers_length]; i++)
    {
        color = actual_output == i ? 1 : 0.5;
        m_window->draw_circle(Pos{ -700.0 + 75 * i,  515 }, 30, Color{ 0.0, 0.0, color });
    }
}


float Drawer::getWeightColor(int layers_n, int j, int k) {
    Layer* layer = &m_preseau->layers[layers_n];
    return layer->weights[j % layer->repeat_factor * m_preseau->m_layerSizes[layers_n + 1] + k] > 0 ? 1.0 : 0.5;
}

void Drawer::DrawLayer(int layers_n) { // layers_n is the id of the layer to be drawn
    NeuralNetwork* preseau = m_preseau;
    Window* window = m_window;
    int n_nodesIn = layers_n == 0 ? 8 :  preseau->m_layerSizes[layers_n]; // the 8 here is just for inputs layers
    for (int nodeIn = 0; nodeIn < n_nodesIn; nodeIn++)
    {
        //color = 0.2 + 0.8 * preseau->layers[i]->weights[j, preseau->m_layerSizes[i+1]];
        float color = 0.5;
        for (int nodeOut = 0; nodeOut < preseau->m_layerSizes[layers_n + 1]; nodeOut++)
        {
            
            if (layers_n == 0 && nodeIn > 6)
                color = getWeightColor(layers_n, 6 + preseau->m_layerSizes[layers_n] - nodeIn, nodeOut);
            else
                color = getWeightColor(layers_n, nodeIn, nodeOut);
            
            window->draw_line(Pos{ -700.0 + 25.0 * nodeIn, -400.0 + (layers_n * 915 / preseau->layers_length) },
                Pos{ -700.0 + 25.0 * nodeOut * (layers_n == preseau->layers_length - 1 ? 3 : 1),
                -400.0 + ((layers_n + 1) * 915 / preseau->layers_length)}, Color{ color, color, color });
        }
        color = layers_n != 0 && preseau->layers[layers_n].activations[nodeIn] >= 0.5 ? 1 : 0.5;
        window->draw_circle(Pos{ -700.0 + 25.0 * nodeIn, -400.0 + (layers_n * 915 / preseau->layers_length) }, 10, Color{ color, color, color });

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