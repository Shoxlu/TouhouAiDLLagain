
#include "Reseau.h"
#include "Input.h"
#include "Hidden_neurone.h"
#include "Personnage.h"
#include "utils.h"
#include <stdio.h>
#include <corecrt_math.h>
#define VK_X 0x58
#define VK_W 0x57

Reseau::~Reseau()
{
    for (int i = 0; i < m_n_inputs; i++)
    {
        delete m_inputs[i];
    }
    for (int i = 0; i < m_n_hidden_neurones; i++)
    {
        delete m_layer[i];
    }
}

void Reseau::input_into_layer()
{
    m_layer = new GenericNeurone * [m_n_inputs];
    for (int i = 0; i < m_n_inputs; i++)
    {
        m_layer[i] = m_inputs[i];
    }
}

Reseau::Reseau(int n_layers, int n_inputs_all, int n_hidden_neurones, Joueur* joueur) : m_n_layers(n_layers), m_n_hidden_neurones(n_hidden_neurones), m_joueur(joueur), m_n_inputs(n_inputs_all)
{
    
    m_n_outputs = 6;
    printf("Inputs: \n");
    Create_Inputs();
    //printf("Inputs cr��es");
    Reset_m_layers();
    m_layer = new GenericNeurone * [n_inputs_all];
    int n_inputs = n_inputs_all;
    input_into_layer();
    printf("Hidden_layers: \n");
    int n_outputs = m_n_hidden_neurones;
    for (int i = 0; i < n_layers; i++)
    {
        if (i == n_layers - 1)
        {
            n_outputs = 6; //6 is the number of outputs
        }
        Create_hidden_layer(n_inputs, m_layer, m_n_hidden_neurones, n_outputs);
        m_layers[i] = m_layer;
        n_inputs = m_n_hidden_neurones;
    }
    /*printf("Outputs: \n");
    weights = create_weights(n_inputs, 1, randint(0, n_inputs - 1));
    Create_hidden_layer(n_inputs, m_layer, m_n_outputs);
    m_outputs = m_layer;*/
    moves[0] = Dir{ VK_DOWN , true };
    moves[1] = Dir{ VK_UP , true };
    moves[2] = Dir{ VK_LEFT , true };
    moves[3] = Dir{ VK_RIGHT, true };
    moves[4] = Dir{ VK_X , true };
    moves[5] = Dir{ VK_SHIFT , true };
    mutation_reseau();
   
}

Reseau::Reseau(int n_layers, int n_inputs_all ,int n_hidden_neurones, GenericNeurone*** layers, Input** inputs, Joueur* joueur) :m_n_layers(n_layers), m_n_hidden_neurones(n_hidden_neurones), m_joueur(joueur), m_n_inputs(n_inputs_all), m_layers(layers)
{
    m_n_outputs = 6;
    Reset_m_layers();
    m_inputs = new Input * [n_inputs_all];
    for (int i = 0; i < n_inputs_all; i++)
    {

        m_inputs[i] = inputs[i];
        m_inputs[i]->mutation_Input();
        //printf("%f, %f, %f \n", inputs[i]->get_offset().x, inputs[i]->get_offset().y, inputs[i]->get_weight());

    }
    int n_inputs = n_inputs_all;
    input_into_layer();
    auto layer_input = m_layer;
    for (int i = 0; i < n_layers; i++)
    {
        m_layer = new GenericNeurone * [n_hidden_neurones];
        for (int j = 0; j < n_hidden_neurones; j++)
        {
            m_layers[i][j] = layers[i][j];
            ((Hidden_neurone*)m_layers[i][j])->mutation_hidden_neurone();
        }
        
        n_inputs = n_hidden_neurones;
        layer_input = m_layers[i];
    }
    
    /*m_outputs = new GenericNeurone * [m_n_outputs];
    for (int j = 0; j < m_n_outputs; j++)
    {
        m_outputs[j] = output[j];
        ((Hidden_neurone*)m_outputs[j])->mutation_hidden_neurone();
    }
    m_layer = new GenericNeurone * [m_n_outputs];*/
    mutation_reseau();
    moves[0] = Dir{ VK_DOWN , true };
    moves[1] = Dir{ VK_UP , true };
    moves[2] = Dir{ VK_LEFT , true };
    moves[3] = Dir{ VK_RIGHT, true };
    moves[4] = Dir{ VK_X , true };
    moves[5] = Dir{ VK_SHIFT , true };
    
}
void Reseau::update()
{
    for (int i = 0; i < m_n_inputs; i++)
    {
        m_inputs[i]->Update();
    }
    for (int i = 0; i < m_n_outputs; i++)
    {
        float activation = 0;
        for (int k = 0; k < m_n_hidden_neurones; k++) {
            activation += m_layers[m_n_layers - 1][k]->get_output(i, k);
        }
        if (1 / (1 + pow(2.71828, -activation)) > 0.5)
        {
            moves[i].released = false;
            m_joueur->move(moves[i]);

        }
        else if (moves[i].released == false)
        {
            moves[i].released = true;
            m_joueur->move(moves[i]);
        }
    }
}

Dir* Reseau::get_moves()
{
    return moves;
}

void Reseau::Reset_m_layers()
{
    GenericNeurone*** layers = nullptr;
    if (m_layers != nullptr)
    {
        layers = m_layers;
    }


    int n_layers = m_n_layers;
    m_layers = new GenericNeurone * *[n_layers];
    for (int i = 0; i < n_layers; i++)
    {
        m_layers[i] = new GenericNeurone * [m_n_hidden_neurones];
    }
    if (layers == nullptr)
    {
        return;
    }
    for (int j = 0; j < n_layers; j++)
    {
        for (int i = 0; i < m_n_hidden_neurones; i++)
        {
            if (layers[j][i] != NULL) {
                m_layers[j][i] = layers[j][i];
            }
        }
    }
}
int Reseau::get_n_inputs()
{
    return m_n_inputs;
}
Joueur* Reseau::get_joueur()
{
    return m_joueur;
}

void Reseau::Create_Inputs()
{
    m_inputs = new Input * [m_n_inputs];
    for (int i = 0; i < m_n_inputs; i++)
    {
        m_inputs[i] = new Input(random_float()*100, random_float()*100, m_n_hidden_neurones,  m_joueur);
    }
}
void Reseau::Create_hidden_layer(int n_inputs, GenericNeurone** inputs, int n, int n_outputs)
{
    m_layer = new GenericNeurone * [n];
    for (int i = 0; i < n; i++)
    {
        weights = create_weights(n_outputs, 1.0, randint(0, n_outputs - 1));
        m_layer[i] = new Hidden_neurone(n_inputs, inputs, n_outputs,weights, m_joueur);
        ((Hidden_neurone*)m_layer[i])->mutation_hidden_neurone();
    }

}

int Reseau::get_n_Hidden_layer() const
{
	return m_n_layers;
}
int Reseau::get_n_Hidden_neurones() const
{
    return m_n_hidden_neurones;
}

GenericNeurone* Reseau::get_Hidden_layer(int i) {
	return m_layer[i];
}

GenericNeurone*** Reseau::get_Hidden_layers()
{
    return m_layers;
}
Input** Reseau::get_inputs()
{
    return m_inputs;
}

void Reseau::mutation_reseau()
{
    double chance = randint(0, 100);
    if (chance < 5)
    {
        //Reseau::mutation_reseau_add_Hidden_layer(); i will skip this for now
    }
    if (randint(0, 100) < 30)
    {
        add_input();
        printf("+1 input \n");
    }
    else if (randint(0, 100) < 5 && m_n_inputs > 1)
    {
       remove_input(randint(0, m_n_inputs-1));
       printf("-1 input \n");
    }
    if (randint(0, 100) < 1)
    {
        add_hidden_neurone();
    }

}
void Reseau::remove_input(int j)
{
    m_n_inputs -= 1;
    int n_inputs = m_n_inputs;
    //delete m_inputs[j];
    if (j != 0)
    {
        GenericNeurone* temp_personnages = m_inputs[0];
        m_inputs[0] = m_inputs[j];
        m_inputs[j] = (Input*)temp_personnages;
    }
    for (int k = 0; k < n_inputs-1; k++)
    {
        m_inputs[k] = m_inputs[k + 1];
    }
    Input** inputs = m_inputs;
    m_inputs = new Input * [n_inputs];
    for (int i = 0; i < n_inputs; i++) {
        m_inputs[i] = inputs[i];
    }
    for (int i = 0; i < m_n_hidden_neurones; i++)
    {
        ((Hidden_neurone*)m_layers[0][i])->change_input((GenericNeurone**)m_inputs, m_n_inputs);
    }
}

void Reseau::add_input()
{
    m_n_inputs += 1;

    Input** inputs = m_inputs;
    int n_inputs = m_n_inputs;
    m_inputs = new Input * [n_inputs];
    for (int i = 0; i < n_inputs-1; i++) {
        m_inputs[i] = inputs[i];
    }
    float* weights = create_weights(m_n_hidden_neurones + 1, 1.0, m_n_hidden_neurones -1);
    m_inputs[n_inputs - 1] = new Input(random_float() * 100, random_float() * 100, m_n_hidden_neurones+1, weights, m_joueur);
    for (int i = 0; i < m_n_hidden_neurones; i++)
    {
        ((Hidden_neurone*)m_layers[0][i])->change_input((GenericNeurone**)m_inputs, n_inputs);
    }
    add_hidden_neurone();
}

//this probably works with more than 1 hidden layer, but not sure
void Reseau::add_hidden_neurone()
{
    m_n_hidden_neurones += 1;
    Reset_m_layers();
    input_into_layer();
    int n_outputs = m_n_hidden_neurones;
    for (int i = 0; i < m_n_layers; i++)
    {
        if (i == m_n_layers - 1) {
            n_outputs = 6;
        }
        weights = create_weights(n_outputs, 1.0, randint(0, n_outputs-1));
        m_layers[i][m_n_hidden_neurones - 1] = new Hidden_neurone(m_n_inputs, m_layer, n_outputs, weights, m_joueur);
        m_layer = m_layers[i];
        if (i != 0)
        {
            for (int j = 0; j < m_n_hidden_neurones-1; j++)
            {
            
                (m_layers[i - 1][j])->change_weights(m_n_hidden_neurones);
                (m_layers[i - 1][j])->change_input(m_layer, m_n_hidden_neurones);
            }  
        }
    }
}

//not implemented
void Reseau::add_hidden_layer()
{
    double chance = randint(0, 100);
    if (chance < 50)
    {
        m_n_layers += 1;
        for (int i = 0; i < m_n_hidden_neurones; i++)
        {
            //m_layers[m_n_layers - 1][i] = new Hidden_neurone(m_n_inputs, m_layer, m_joueur);
        }
    }
    else if(m_n_hidden_neurones > 50 && m_n_layers > 1)
    {
        delete_Hidden_layer(randint(0, m_n_layers - 1));
    }
}

void Reseau::delete_Hidden_layer(int j)
{

    for (int i = 0; i < m_n_hidden_neurones; i++)
    {
        delete m_layers[j][i];
    }
    GenericNeurone** temp_personnages = m_layers[0];
    m_layers[0] = m_layers[j];
    m_layers[j] = temp_personnages;
    for (int k = 0; k < m_n_layers - 1; k++)
    {
        m_layers[k] = m_layers[k + 1];
    }
    m_n_layers -= 1;
}
