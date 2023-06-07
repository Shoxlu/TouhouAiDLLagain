
#include "GenerationPerso.h"
#include "Joueur.h"
#include "NeuralNetwork.h"
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
extern NeuralNetwork* preseau;

GenerationJoueur::GenerationJoueur(const int n_systemes) : previous_miss_count(0), m_n_systemes(n_systemes), m_n_generation(1), m_n_all_systemes(n_systemes), joueur_actuel(0), best_joueur_ids(nullptr), n_best_joueurs(n_systemes /2)
{
    isPlaying = true;
    m_rewards = new int[n_systemes];
    m_joueurs = new Joueur*[n_systemes];
    for (int i = 0; i < n_systemes; i++)
    {
        m_joueurs[i] = new Joueur();
    } 
    preseau = m_joueurs[0]->m_reseau;
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
    printf("Begin Joueurs creation\n");
    SortBestJoueurs();
    n_best_joueurs = m_n_all_systemes / 2;//idk if it is useful but VS was complaining about the for loop
    for (int i = n_best_joueurs; i < m_n_all_systemes; i++)
    {
        m_joueurs[i]->Reset(m_joueurs[i - n_best_joueurs]->m_reseau);;
    }
    printf("Joueurs créés\n");
    m_n_systemes = m_n_all_systemes;
    printf("N generation: %d \n", m_n_generation);
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

int GenerationJoueur::update() {
    if (joueurMort())
    {   
        m_rewards[joueur_actuel] = m_joueurs[joueur_actuel]->m_reward;
        joueur_actuel += 1;
        if (joueur_actuel >= m_n_all_systemes)
        {
            printf("Fin_generation\n");
            newGeneration();
        }
        HANDLE ResetThread = CreateRemoteThread(hprocess,NULL,NULL,(LPTHREAD_START_ROUTINE)ResetGame, NULL,NULL,NULL);
        isPlaying = false;
        preseau = NULL;
        
    }
    else if (isPlaying == true) {
        return m_joueurs[joueur_actuel]->update();
    }
    if (isPlaying == false && global_ptr->time_in_stage == 1) {
        printf("New actual player id: %d \n", joueur_actuel);
        isPlaying = true;
        preseau = m_joueurs[joueur_actuel]->m_reseau;
    }
    return 0;
}


void GenerationJoueur::update_() {
    m_joueurs[joueur_actuel]->update_();
}

void ResetGame()
{
    printf("Reset game.\n");
    ReleaseAllInputs();
    press(VK_W, 1);
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
void GenerationJoueur::SortBestJoueurs() {
    int best_id;
    for (int i = 0; i < n_best_joueurs; i++) {
        int best_reward = m_rewards[i];
        best_id = i;
        for (int j = 0; j < m_n_all_systemes; j++) {
            if (m_rewards[j] >= best_reward) {
                best_reward = m_rewards[j];
                best_id = j;
            }
        }
        Joueur* temp = m_joueurs[i];
        m_joueurs[i] = m_joueurs[best_id];
        m_joueurs[best_id] = temp;
        m_rewards[best_id] = 0;
    }
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