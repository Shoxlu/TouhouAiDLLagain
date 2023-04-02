
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

/*extern*/ zGlobals* global_ptr;
extern zPlayer player_ptr;

GenerationJoueur::GenerationJoueur(const int n_systemes): previous_miss_count(0), m_n_systemes(n_systemes), m_n_generation(0), m_best_reward(0), m_n_all_systemes(n_systemes), joueur_actuel(0)
{
    //commandList = new List<Reseau*>();
    //printf("Command list created");
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
    m_joueur[0] = m_best_joueur;
    best_joueur_id = 0;
    printf("Begin Joueurs creation\n");
    for (int i = 1; i < m_n_all_systemes; i++)
    {
        m_joueur[i]->Reset_joueur(m_best_joueur->get_reseau());
    }
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

void GenerationJoueur::change_n_systemes(int nombre)
{
    m_n_systemes = nombre;
}

int GenerationJoueur::get_n_generation()
{
    return m_n_generation;
}

int GenerationJoueur::joueur_mort()
{
    //printf("joueur_mort check\n");
    //std::cout << previous_miss_count << " previous miss \n";
    //std::cout << global_ptr->miss_count << "actual miss \n";
    
    if (global_ptr->miss_count > previous_miss_count)
    {
        previous_miss_count = global_ptr->miss_count;
        printf("Joueur est mort\n");
        return 1;
    }
    previous_miss_count = global_ptr->miss_count;
    return 0;
}


void GenerationJoueur::kill_joueur(int j)
{
    //commandList->append(m_joueur[j]->get_reseau());
    delete m_joueur[j];

}

void GenerationJoueur::fin_generation()
{
    printf("Fin_generation\n");
    //execute_command_list();
    new_generation();
}

void GenerationJoueur::execute_command_list()
{

    Reseau* reseau = nullptr;
    for (int i = 0; i < commandList->length(); i++)
    {
        reseau = commandList->get(i);
        //reseau = (Reseau*)commandList->get(i);
        if (reseau != nullptr)
        {
            delete reseau;
        }
        
    }
    delete commandList;
    commandList = new List<Reseau*>();
}

void GenerationJoueur::update() {
  
    m_joueur[joueur_actuel]->update();
   if (joueur_mort())
   {

       //si score du joueur est meilleur que le meilleur score enregistré: alors mettre m_best_perso à m_joueur[joueur_actuel]
       std::cout << m_joueur[joueur_actuel]->get_reward() << "\n";
       std::cout << m_best_reward << "\n";
       if (m_joueur[joueur_actuel]->get_reward() >= m_best_reward)
       {
           printf("best joueur\n");
           best_joueur_id = joueur_actuel;
           m_best_joueur = m_joueur[joueur_actuel];
           m_best_reward = m_best_joueur->get_reward();
           
       }
       else {
           printf("not best joueur\n"); // i can actually get rid of this, but for debug purpose i won't
       }
       joueur_actuel += 1;
       printf("Reset \n");
       Release_All_Inputs();
       press(VK_ESCAPE, 0);
       Sleep(500);
       press(VK_ESCAPE, 1);
       press(VK_R, 0);
       Sleep(500);
       press(VK_UP, 0);
       Sleep(100);
       press(VK_UP, 1);
       Sleep(100);
       press(VK_W, 0);
       Sleep(30);
       press(VK_W, 1);
       press(VK_R, 1);
       
       press(VK_W, 0);
       printf("%d \n", joueur_actuel);

   }
   
   if (joueur_actuel >= m_n_all_systemes)
   {
       fin_generation();
   }
}

