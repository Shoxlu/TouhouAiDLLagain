
#include "GenerationPerso.h"
#include "Personnage.h"
#include "Reseau.h"
#include <iostream>
#include "utils.h"

#define VK_W 0x57
#define VK_R 0x52
#define VK_D 0x44
#define VK_C 0x43
#define VK_X 0x58

extern zGlobals* global_ptr;
extern zPlayer player_ptr;
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
    std::cout << "N generation:" << m_n_generation << "\n";
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
        if(best_joueur_id != i)
        {
            m_joueur[i]->Reset_joueur(m_best_joueur->get_reseau());
        }
            
    }
    auto temp = m_joueur[0];
    m_joueur[0] = m_joueur[best_joueur_id];
    m_joueur[best_joueur_id] = temp;
    printf("Joueurs créés\n");
    m_n_systemes = m_n_all_systemes;
    std::cout << "N generation:" << m_n_generation << "\n";
    
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

void GenerationJoueur::execute_command_list()
{

    Reseau* reseau = nullptr;
    for (int i = 0; i < commandList.length(); i++)
    {
        reseau = commandList.get(i);
        //reseau = (Reseau*)commandList->get(i);
        if (reseau != nullptr)
        {
            delete reseau;
        }
        
    }
    commandList = List();
}

void GenerationJoueur::update() {
    m_joueur[joueur_actuel]->update();
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
}

