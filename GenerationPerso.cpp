
#include "GenerationPerso.h"
#include "Personnage.h"
#include "Reseau.h"
#include "GenericNeurone.h"
#include "Hidden_neurone.h"
#include "Input.h"
#include <iostream>
#include "utils.h"

#define VK_W 0x57
#define VK_R 0x52
#define VK_D 0x44
#define VK_C 0x43
#define VK_X 0x58

extern zGlobals* global_ptr;
extern zPlayer* player_ptr;
extern int previous_time;
GenerationJoueur::GenerationJoueur(const int n_systemes): previous_miss_count(0), m_n_systemes(n_systemes), m_n_generation(0), m_best_reward(0), m_n_all_systemes(n_systemes), joueur_actuel(0)
{
    first_generation();
}

void GenerationJoueur::first_generation()
{
    joueur_actuel = 0;
    m_n_generation += 1;
    m_joueur = new Joueur*[m_n_all_systemes];
    for (int i = 0; i < m_n_all_systemes; i++)
    {
        m_joueur[i] = new Joueur();
    }
    printf("Joueurs créés\n");
    best_joueur_id = 0;
    m_best_joueur = m_joueur[0];
    m_n_systemes = m_n_all_systemes;
    printf("N generation: %d \n", m_n_generation);
}

void GenerationJoueur::new_generation()
{
    printf("New generation \n");
    joueur_actuel = 0;
    m_n_generation += 1;
    m_best_reward = 0;
    m_best_joueur->set_reward(0);
    printf("Begin Joueurs creation\n");
    
    for (int i = 0; i < m_n_all_systemes; i++)
    {
        auto new_reseau = Create_mixed_reseau(m_joueur[i]);
        if(best_joueur_id != i)
        {
            m_joueur[i]->Reset_joueur(new_reseau);
        }
            
    }
    auto temp = m_joueur[0];
    m_joueur[0] = m_joueur[best_joueur_id];
    m_joueur[best_joueur_id] = temp;
    printf("Joueurs créés\n");
    m_n_systemes = m_n_all_systemes;
    printf("N generation: %d \n", m_n_generation);
}

Reseau* GenerationJoueur::Create_mixed_reseau(Joueur* joueur)
{
    //Trouver un parent
    Reseau* parent = m_joueur[randint(0, m_n_all_systemes-1)]->get_reseau();
    //trouver les caractéritiques à lui prendre
    Input** parent_inputs = parent->get_inputs();
    GenericNeurone*** parent_layers = parent->get_Hidden_layers();

    Reseau* best_joueur_reseau = m_best_joueur->get_reseau();
    Input** best_joueur_inputs = best_joueur_reseau->get_inputs();
    GenericNeurone*** best_joueur_layers = best_joueur_reseau->get_Hidden_layers();

    int n_inputs = best_joueur_reseau->get_n_inputs();
    int n_layers = best_joueur_reseau->get_n_Hidden_layer();
    int n_hidden_neurones = best_joueur_reseau->get_n_Hidden_neurones();

    random_network_charac(&n_inputs, &n_layers, &n_hidden_neurones, parent);

    Input** inputs = new Input * [n_inputs];
    for (int i = 0; i < n_inputs; i++)
    {
        inputs[i] = take_random_input(parent, best_joueur_reseau, parent_inputs, best_joueur_inputs, n_hidden_neurones, joueur, i);
    }
    GenericNeurone*** layers = new GenericNeurone ** [n_layers];
    int temp_n_inputs = n_inputs;
    auto temp_inputs = input_into_layer(inputs, temp_n_inputs);
    for (int i = 0; i < n_layers; i++) {
        layers[i] = new GenericNeurone * [n_hidden_neurones];
        int n_outputs;
        if(i == n_layers-1)
        {
            n_outputs = 6;
        }
        else {
            n_outputs = n_hidden_neurones;
        }
        for (int j = 0; j < n_hidden_neurones; j++)
        {
            float* weights = take_random_hidden_neurone_weights(n_outputs, parent, best_joueur_reseau, i, j);
            layers[i][j] = new Hidden_neurone(temp_n_inputs, temp_inputs, n_outputs, weights, joueur);
        }
        temp_n_inputs = n_hidden_neurones;
        temp_inputs = layers[i];
    }
    //remplacer les caractéritiques de ce réseau avec celles qui sont prises chez le parent
    //créer un nouveau réseau avec les nouvelles charactéristiques
    Reseau* new_reseau = new Reseau(n_layers, n_inputs, n_hidden_neurones, layers, inputs, joueur);
    //renvoyer le réseau
    return new_reseau;
}
void GenerationJoueur::random_network_charac(int* pn_inputs,int* pn_layers, int* pn_hidden_neurones, Reseau* parent)
{
    if (random_float_positive() >= 0.5)
    {
        *pn_inputs = parent->get_n_inputs();
    }
    if (random_float_positive() >= 0.5)
    {
        *pn_layers = parent->get_n_Hidden_layer();
    }
    if (random_float_positive() > 0.5)
    {
       *pn_hidden_neurones = parent->get_n_Hidden_neurones();
    }
}
float* GenerationJoueur::take_random_hidden_neurone_weights(int n, Reseau* parent, Reseau* best_joueur, int i, int j)
{
    float* weights = new float[n];
    GenericNeurone*** parent_layers = parent->get_Hidden_layers();
    GenericNeurone*** best_layers = best_joueur->get_Hidden_layers();
    if (random_float_positive() > 0.5 && i < parent->get_n_Hidden_layer() && j < parent->get_n_Hidden_neurones())
    {
        
        parent_layers[i][j]->change_weights(n);
        weights = ((Hidden_neurone*)parent_layers[i][j])->get_weights();
    }
    else if (i < best_joueur->get_n_Hidden_layer() && j < best_joueur->get_n_Hidden_neurones()) {
        best_layers[i][j]->change_weights(n);
        weights = ((Hidden_neurone*)best_layers[i][j])->get_weights();
    }
    else if (i < parent->get_n_Hidden_layer() && j < parent->get_n_Hidden_neurones()) {
        parent_layers[i][j]->change_weights(n);
        weights = ((Hidden_neurone*)parent_layers[i][j])->get_weights();
    }
    else {
        weights = create_weights(n, 1, randint(0, n-1));
    }
    return weights;
}
Input* GenerationJoueur::take_random_input(Reseau* parent, Reseau* best_joueur, Input** parent_inputs, Input** best_joueur_inputs, int n_hidden_neurones, Joueur* joueur, int i)
{
    if (random_float_positive() > 0.5 && i < parent->get_n_inputs())
    {
        Pos offset = parent_inputs[i]->get_offset();
        return new Input(offset.x, offset.y, n_hidden_neurones, parent_inputs[i]->get_weights(), joueur);
    }
    else if (i < best_joueur->get_n_inputs()) {
        Pos offset = best_joueur_inputs[i]->get_offset();
        return new Input(offset.x, offset.y, n_hidden_neurones,best_joueur_inputs[i]->get_weights(), joueur);
    }
    else if (i < parent->get_n_inputs()) {
        Pos offset = parent_inputs[i]->get_offset();
        return new Input(offset.x, offset.y, n_hidden_neurones,parent_inputs[i]->get_weights(), joueur);
    }
    else {
        return  new Input(random_float() * 100, random_float() * 100, n_hidden_neurones, joueur);
    }
}
GenericNeurone** GenerationJoueur::input_into_layer(Input** inputs, int n_inputs)
{
    auto layer = new GenericNeurone * [n_inputs];
    for (int i = 0; i < n_inputs; i++)
    {
        layer[i] = inputs[i];
    }
    return layer;
}

void GenerationJoueur::Get_parent_charac()
{
    
    //renvoyer
}
Joueur** GenerationJoueur::get_joueurs() {
    return m_joueur;
}

void GenerationJoueur::delete_generation()
{
    delete[] m_joueur;
}

int GenerationJoueur::get_n_systemes()
{
    return m_n_systemes;
}

int GenerationJoueur::get_joueur_actuel()
{
    return joueur_actuel;
}

void GenerationJoueur::change_n_systemes(int nombre)
{
    m_n_systemes = nombre;
}

int GenerationJoueur::get_n_generation()
{
    return m_n_generation;
}

bool GenerationJoueur::joueur_mort()
{
    if (global_ptr->miss_count > previous_miss_count)
    {
        previous_miss_count = global_ptr->miss_count;
        return true;
    }
    previous_miss_count = global_ptr->miss_count;
    return false;
}


void GenerationJoueur::kill_joueur(int j)
{
    delete m_joueur[j];
}

void GenerationJoueur::fin_generation()
{
    printf("Fin_generation\n");
    new_generation();
}

//void GenerationJoueur::execute_command_list()
//{
//
//    Reseau* reseau = nullptr;
//    for (int i = 0; i < commandList.length(); i++)
//    {
//        reseau = commandList.get(i);
//        //reseau = (Reseau*)commandList->get(i);
//        if (reseau != nullptr)
//        {
//            delete reseau;
//        }
//        
//    }
//    commandList = List();
//}

void GenerationJoueur::update() {
    
    if (joueur_mort())
    {
       //si score du joueur est meilleur que le meilleur score enregistré: alors mettre m_best_perso à m_joueur[joueur_actuel]
        printf("Actual player reward : %d \n", m_joueur[joueur_actuel]->get_reward());
        printf("Best player reward : %d \n", m_best_reward);
        if (m_joueur[joueur_actuel]->get_reward() >= m_best_reward)
        {
            printf("Best player. \n");
            best_joueur_id = joueur_actuel;
            m_best_joueur = m_joueur[joueur_actuel];
            m_best_reward = m_best_joueur->get_reward();
           
        }
        else {
            printf("Not best player. \n"); // i can actually get rid of this, but for debug purpose i won't
        }
        joueur_actuel += 1;
        printf("Reset game.\n");
        Release_All_Inputs();
        if (joueur_actuel >= m_n_all_systemes)
        {
            fin_generation();
        }
        press(VK_ESCAPE, 0);
        Sleep(500);
        press(VK_ESCAPE, 1);
        press(VK_R, 0);
        Sleep(1000);
        press(VK_UP, 0);
        Sleep(300);
        press(VK_UP, 1);
        Sleep(300);
        press(VK_W, 0);
        Sleep(30);
        press(VK_W, 1);
        press(VK_R, 1);
        press(VK_W, 0);
        printf("New actual player id: %d \n", joueur_actuel);
        global_ptr->time_in_stage = 0;
        previous_time = 0;
    }
    else {
        m_joueur[joueur_actuel]->update();
    }
}

