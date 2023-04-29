
#include "GenerationPerso.h"
#include "Joueur.h"
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
extern BYTE frame_skip;
extern HANDLE hprocess;
extern bool isRendering;
GenerationJoueur::GenerationJoueur(const int n_systemes) : previous_miss_count(0), m_n_systemes(n_systemes), m_n_generation(1), m_n_all_systemes(n_systemes), joueur_actuel(0), best_joueur_ids(nullptr), n_best_joueurs(n_systemes /2)
{
    isPlaying = true;
    m_rewards = new int[n_systemes];
    m_joueurs = new Joueur[n_systemes];
    printf("Joueurs créés\n");
    printf("N generation: %d \n", m_n_generation);
}

GenerationJoueur::~GenerationJoueur()
{
    if(m_joueurs)
        delete[] m_joueurs;
    if (m_rewards)
        delete[] m_rewards;
    if (best_joueur_ids)
        delete[] best_joueur_ids;
}

void GenerationJoueur::newGeneration()
{
    printf("New generation \n");
    joueur_actuel = 0;
    m_n_generation += 1;
    best_joueur_ids = getBestJoueurs();
    printf("Begin Joueurs creation\n");
    Joueur* joueurs = new Joueur [m_n_all_systemes];
    n_best_joueurs = m_n_all_systemes / 2;//idk if it is useful but VS was complaining about the for loop
    for (int i = 0; i < n_best_joueurs; i++) {
        joueurs[i] = m_joueurs[best_joueur_ids[i]];
    }
    for (int i = n_best_joueurs; i < m_n_all_systemes; i++)
    {
        joueurs[i] = m_joueurs[i];
    }
    //delete[] m_joueurs;
    m_joueurs = joueurs;
    for (int i = n_best_joueurs; i < m_n_all_systemes; i++)
    {
        m_joueurs[i].Reset(m_joueurs[best_joueur_ids[i - n_best_joueurs]].m_reseau);
    }
    printf("Joueurs créés\n");
    m_n_systemes = m_n_all_systemes;
    printf("N generation: %d \n", m_n_generation);
}

void GenerationJoueur::delete_generation()
{
    delete[] m_joueurs;
}

bool GenerationJoueur::joueurMort()
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
    //delete m_joueurs[j];
}

void GenerationJoueur::update() {
    if (joueurMort())
    {   
        m_rewards[joueur_actuel] = m_joueurs[joueur_actuel].m_reward;
        joueur_actuel += 1;
        if (joueur_actuel >= m_n_all_systemes)
        {
            printf("Fin_generation\n");
            newGeneration();
        }
        ResetThread = CreateRemoteThread(
            hprocess,
            NULL,
            NULL,
            (LPTHREAD_START_ROUTINE)ResetGame,
            NULL,
            NULL,
            NULL
        );
        isPlaying = false;
        
    }
    else if (isPlaying == true) {
        m_joueurs[joueur_actuel].update();
    }
    if (isPlaying == false && global_ptr->time_in_stage == 1) {
        printf("New actual player id: %d \n", joueur_actuel);
        isPlaying = true;
    }
}
void ResetGame()
{
    printf("Reset game.\n");
    Release_All_Inputs();
    press(VK_ESCAPE, 0);
    Sleep((500 / (frame_skip + 1))*isRendering+100*!isRendering);
    press(VK_ESCAPE, 1);
    press(VK_R, 0);
    Sleep((1000 / (frame_skip + 1))* isRendering+100 * !isRendering);
    press(VK_UP, 0);
    Sleep((300 / (frame_skip + 1))* isRendering+10 * !isRendering);
    press(VK_UP, 1);
    Sleep((300 / (frame_skip + 1))* isRendering+20 * !isRendering);
    press(VK_W, 0);
    Sleep(30);
    press(VK_W, 1);
    press(VK_R, 1);
    press(VK_W, 0);
    global_ptr->time_in_stage = 0;
    previous_time = 0;
}

int* GenerationJoueur::getBestJoueurs() {
    int* bestJoueurs = new int[n_best_joueurs];
    int best_id;
    for (int i = 0; i < n_best_joueurs; i++) {
        int best_reward = m_rewards[i];
        best_id = i;
        for (int j = 0; j < m_n_all_systemes; j++) {
            if (m_rewards[j] >= best_reward ) {
                best_reward = m_rewards[j];
                best_id = j;
            }
        }
        bestJoueurs[i] = best_id;
        printf("best_rewards_ids[%d] = %d, m_rewards[%d] = %d \n", i, bestJoueurs[i], best_id, m_rewards[best_id]);
        m_rewards[best_id] = 0;
    }
    printf("\n");
    return bestJoueurs;
}

int* GenerationJoueur::getBestRewards() {
    int* best_rewards = new int[n_best_joueurs];
    int n = 0;
    for (int i = 0; i < m_n_all_systemes; i++) {
        for (int j = 0; j < m_n_all_systemes-1; j++) {
            if (m_rewards[i] > m_rewards[i + 1]) {
                int temp = m_rewards[i];
                m_rewards[i] = m_rewards[i + 1];
                m_rewards[i + 1] = temp;
            }
        }
    }
    for (int i = 0; i < n_best_joueurs; i++) {
        best_rewards[i] = m_rewards[m_n_all_systemes -1 - i];
    }
    return best_rewards;
}