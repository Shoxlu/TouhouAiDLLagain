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
    float color = 1;
    if (!preseau || !m_window) {
        //printf("Missing window or network \n");
        return;
    }
    int layers_length = preseau->layers_length;

    for (int i = 0; i < layers_length; i++)
    {
        DrawLayer(i);
    }
    for (int i = 0; i < preseau->m_layerSizes[layers_length]; i++)
    {
        if (preseau->layers[layers_length - 1].activations && actual_output == i)
            color = 1;
        else
            color = 0.5;
        m_window->draw_circle(Pos{ -700.0 + 75 * i,  515 }, 30, Color{ 0.0, 0.0, color });
    }
}


float Drawer::getWeightColor(int layers_n, int j, int k) {
    Layer* layer = &m_preseau->layers[layers_n];
    if (layer->weights[j % layer->repeat_factor * m_preseau->m_layerSizes[layers_n + 1] + k] > 0) {
        return 1.0;
    }
    return 0.5;
}

void Drawer::DrawLayer(int n) { // n is the id of the layer to be drawn
    NeuralNetwork* preseau = m_preseau;
    Window* window = m_window;

    for (int j = 0; j < preseau->m_layerSizes[n]; j++)
    {
        //color = 0.2 + 0.8 * preseau->layers[i]->weights[j, preseau->m_layerSizes[i+1]];
        float color = 0.5;
        for (int k = 0; k < preseau->m_layerSizes[n + 1]; k++)
        {
            color = getWeightColor(n, j, k);
            window->draw_line(Pos{ -700.0 + 25.0 * j, -400.0 + (n * (915 / preseau->layers_length)) },
                Pos{ -700.0 + 25.0 * k * (n == preseau->layers_length - 1 ? 3 : 1),
                -400.0 + ((n + 1) * (915 / preseau->layers_length)) }, Color{ color, color, color });
        }
        color = 0.5;
        if (preseau->layers[n].activations && n != 0 && preseau->layers[n].activations[j] >= 0.5) {
            color = 1;
        }
        window->draw_circle(Pos{ -700.0 + 25.0 * j, -400.0 + (n * (915 / preseau->layers_length)) }, 10, Color{ color, color, color });

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
    if (!m_window)
        printf("no Window ? \n");
    m_window->update();
}