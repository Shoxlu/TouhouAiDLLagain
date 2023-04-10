
#include "Reseau.h"
#include "Input.h"
#include "Hidden_neurone.h"
#include "Personnage.h"
#include "utils.h"
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
    Create_Inputs();
    //printf("Inputs créées");
    Reset_m_layers();
    m_layer = new GenericNeurone * [n_inputs_all];
    int n_inputs = n_inputs_all;
    input_into_layer();
    weights = new float[n_inputs_all];
    for (int i = 0; i < n_inputs_all; i++)
    {
        weights[i] = 1.0;
    }
    for (int i = 0; i < n_layers; i++)
    {
        Create_hidden_layer(n_inputs, m_layer, m_n_hidden_neurones);
        m_layers[i] = m_layer;
        n_inputs = m_n_hidden_neurones;
    }
    for (int i = 0; i < n_inputs_all; i++)
    {
        weights[i] = 0.1;
    }
    Create_hidden_layer(n_inputs, m_layer, m_n_outputs);
    m_outputs = m_layer;
    moves[0] = Dir{ VK_DOWN };
    moves[1] = Dir{ VK_UP };
    moves[2] = Dir{ VK_LEFT };
    moves[3] = Dir{ VK_RIGHT };
    moves[4] = Dir{ VK_X };
    moves[5] = Dir{ VK_SHIFT };
    mutation_reseau();
   
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

Reseau::Reseau(int n_layers, int n_inputs_all ,int n_hidden_neurones, GenericNeurone*** layers, Input** inputs, GenericNeurone** output, Joueur* joueur) :m_n_layers(n_layers), m_n_hidden_neurones(n_hidden_neurones), m_joueur(joueur), m_n_inputs(n_inputs_all), m_outputs(output), m_layers(layers)
{
    //printf("New Reseau\n");
    m_n_outputs = 6;
    Reset_m_layers();
    m_inputs = new Input * [n_inputs_all];
    for (int i = 0; i < n_inputs_all; i++)
    {

        m_inputs[i] = new Input(inputs[i]->get_offset().x, inputs[i]->get_offset().y, inputs[i]->get_weight(), m_joueur);
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
            m_layer[j] = new Hidden_neurone(n_inputs, layer_input, ((Hidden_neurone*)layers[i][j])->get_weights(), m_joueur);
            ((Hidden_neurone*)m_layer[j])->mutation_hidden_neurone();
            m_layers[i][j] = m_layer[j];
        }
        
        n_inputs = n_hidden_neurones;
        m_layers[i] = m_layer;
        layer_input = m_layer;
    }
    
    m_layer = new GenericNeurone * [m_n_outputs];
    for (int j = 0; j < m_n_outputs; j++)
    {
        m_layer[j] = new Hidden_neurone(n_hidden_neurones, layer_input, ((Hidden_neurone*)output[j])->get_weights(), m_joueur);
        ((Hidden_neurone*)m_layer[j])->mutation_hidden_neurone();
    }
    m_outputs = m_layer;
    m_layer = new GenericNeurone * [m_n_outputs];
    mutation_reseau();
    moves[0] = Dir{ VK_DOWN };
    moves[1] = Dir{ VK_UP };
    moves[2] = Dir{ VK_LEFT };
    moves[3] = Dir{ VK_RIGHT };
    moves[4] = Dir{ VK_X };
    moves[5] = Dir{ VK_SHIFT };
    
}
void Reseau::update()
{
    for (int i = 0; i < m_n_inputs; i++)
    {
        m_inputs[i]->Activation();
    }
    for (int j = 0; j < m_n_layers; j++)
    {
        for (int i = 0; i < m_n_hidden_neurones; i++) {
            m_layers[j][i]->Activation();
        }
    }
    for (int i = 0; i < m_n_outputs; i++)
    {
        m_outputs[i]->Activation();
        if (m_outputs[i]->get_output() >= 1)
        {
            m_joueur->move(moves[i], false);
        }
        else
        {
            m_joueur->move(moves[i], true);
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

        m_inputs[i] = new Input(random_float()*100, random_float()*100, m_joueur);

    }
}
void Reseau::Create_hidden_layer(int n_inputs, GenericNeurone** inputs, int n)
{
    m_layer = new GenericNeurone * [n];
    for (int i = 0; i < n; i++)
    {
        m_layer[i] = new Hidden_neurone(n_inputs, inputs, weights, m_joueur);
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
GenericNeurone** Reseau::get_outputs()
{
    return m_outputs;
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
        mutation_inputs();
    }
    else if (randint(0, 100) < 30 && m_n_inputs > 1)
    {
       delete_Inputs(randint(0, m_n_inputs-1));
    }
    for (int i = 0; i < m_n_inputs; i++)
    {
        m_inputs[i]->mutation_Input();
    }
    if (randint(0, 100) < 5)
    {
        mutation_Hidden_layers();
    }

}
void Reseau::delete_Inputs(int j)
{
    m_n_inputs -= 1;
    int n_inputs = m_n_inputs;

    Reset_m_inputs();
    delete m_inputs[j];
    if (j != 0)
    {
        GenericNeurone* temp_personnages = m_inputs[0];
        m_inputs[0] = m_inputs[j];
        m_inputs[j] = (Input*)temp_personnages;
    }
    for (int k = 0; k < n_inputs - 1; k++)
    {
        m_inputs[k] = m_inputs[k + 1];
    }
    
    for (int i = 0; i < m_n_hidden_neurones; i++)
    {
        ((Hidden_neurone*)m_layers[0][i])->change_weights(m_n_inputs);
        ((Hidden_neurone*)m_layers[0][i])->change_input((GenericNeurone**)m_inputs, m_n_inputs);
    }
}

void Reseau::mutation_inputs()
{
    m_n_inputs += 1;

    Reset_m_inputs();

    m_inputs[m_n_inputs - 1] = new Input(random_float() * 100, random_float() * 100, m_joueur);
    for (int i = 0; i < m_n_hidden_neurones; i++)
    {
        ((Hidden_neurone*)m_layers[0][i])->change_weights(m_n_inputs);
        ((Hidden_neurone*)m_layers[0][i])->change_input((GenericNeurone**)m_inputs, m_n_inputs);
    }
    mutation_Hidden_layers();
}
void Reseau::Reset_m_inputs()
{
    Input** inputs = m_inputs;
    int n_inputs = m_n_inputs;
    m_inputs = new Input * [n_inputs];
    for (int i = 0; i < n_inputs; i++) {
        if (inputs[i] != NULL)
        {
            m_inputs[i] = inputs[i];
        }
    }
}
void Reseau::mutation_Hidden_layers()
{
    m_n_hidden_neurones += 1;
    Reset_m_layers();
    input_into_layer();
    for (int i = 0; i < m_n_layers; i++)
    {
        m_layers[i][m_n_hidden_neurones - 1] = new Hidden_neurone(m_n_inputs, m_layer, m_joueur);
        m_layer = m_layers[i];
    }
    for (int i = 0; i < m_n_outputs; i++)
    {
        ((Hidden_neurone*)m_outputs[i])->change_weights(m_n_hidden_neurones);
        ((Hidden_neurone*)m_outputs[i])->change_input(m_layer, m_n_hidden_neurones);
    }
}

void Reseau::mutation_reseau_add_Hidden_layer()
{
    double chance = randint(0, 100);
    if (chance < 50)
    {
        m_n_layers += 1;
        for (int i = 0; i < m_n_hidden_neurones; i++)
        {
            m_layers[m_n_layers - 1][i] = new Hidden_neurone(m_n_inputs, m_layer, m_joueur);
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
