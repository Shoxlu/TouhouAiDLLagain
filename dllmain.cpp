// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "pch.h"
#include <iostream>

#include "utils.h"
#include "GenerationPerso.h"
#include "NeuralNetwork.h"
#include "Joueur.h"
#include "InputHelper.h"
#include "dllmain.h"
#include "Window.h"

#include <GLFW/glfw3.h>
#include <thread>
//#include <thread>


#pragma warning(disable : 4996)

using namespace std;


#define VK_W 0x57
#define VK_R 0x52
#define VK_D 0x44
#define VK_C 0x43
#define VK_X 0x58

extern GenerationJoueur* generation;
extern zPlayer* player_ptr;
extern zBulletManager* Bullet_PTR;
extern zGlobals* global_ptr;
GenerationJoueur* generation;
InputHelper* pinputHelper;

const int NbrePerso_generation = 1000;
int previous_time;
Window* window;
HANDLE hprocess;
int global_int; //idk lol
NeuralNetwork* preseau;
void update();
void init();
void render_frame();
void render_frameSub();

BOOL APIENTRY DllMain(HMODULE module, DWORD reasonForCall, LPVOID reserved)
{
  
    switch (reasonForCall)
    {
    case DLL_PROCESS_ATTACH:
    {
        AllocConsole();
        auto a = freopen("CONIN$", "r", stdin);
        a = freopen("CONOUT$", "w", stdout);
        SetConsoleTitle(L"Th18 IA");
        printf("Preparing to run........\n");
        Sleep(1000);
        printf("DLL loaded!\n");     
        auto id = GetCurrentProcessId();
        hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
        /*auto new_thread1 = CreateRemoteThread(
            hprocess,
            NULL,
            NULL,
            (LPTHREAD_START_ROUTINE)init,
            NULL,
            NULL,
            NULL
        );*/ //Injection rework : delete this
        //if (new_thread1 == NULL)//Injection rework : delete this
        //{
        //    printf("Fail creating thread \n");//Injection rework : delete this
        //}
        //Injection rework :
        //Inject "init" jump, at the beginning of th18 WinMain function
        //find init address
        auto init_add = init;
        // Write memory at the good address (ProcessWriteProcessMem)
        patch_call(0x004712D9, init);
        patch_call(0x00471C2A, update);
        BYTE patch[] = {0x90};
        BYTE patch1[] = {0x90, 0x90, 0x90};
        writeMemory(0x4712DE, patch, sizeof(patch));
        writeMemory(0x00471C2A + 0x5, patch1, sizeof(patch1));
        /*WriteProcessMemory(hprocess, "\x00\x47\x12\x70", buffer, sizeof(buffer), NULL);
        WriteProcessMemory(hprocess, "\x00\x47\x12\x70", buffer, sizeof(buffer), NULL);*/
        //Inject "update" jump, in the main loop 
        //find "update" address
        //Create bytes buffer (jmp <update>)
        // Write memory at the good address (ProcessWriteProcessMem)
        printf("End on init \n");
        
    }
    case DLL_PROCESS_DETACH:
        //delete generation;
        speedUpGame(0);
        Release_All_Inputs();
    }
    return TRUE;
}

void init()
{
    LoadLibraryW(L"opengl32.dll");
    srand(time(0));
    //Sleep(10000);
    if (window == NULL)
    {
        //window = new Window();
    }
    pinputHelper = new InputHelper();
    generation = new GenerationJoueur(NbrePerso_generation);
    
    //update();//Injection rework: delete this
}

void update()
{
    previous_time = 0;
    auto joueurs = generation->m_joueurs;
    player_ptr = *(zPlayer**)0x4CF410;
    Bullet_PTR = *(zBulletManager**)0x4CF2BC;
    global_ptr = (zGlobals*)0x4cccc0;
    
    if (global_ptr->time_in_stage > previous_time)//Injection Rework, maybe delete this
    {
        previous_time = global_ptr->time_in_stage;
        if (player_ptr != NULL)
        {
            generation->update();
            if (GetKeyState(VK_BACK) & 0x00000001)
            {
                speedUpGame(10);//Injection Rework, maybe that will useless, to be replaced with a function that prevents game's refreshing
            }
            else if (GetKeyState(VK_BACK) == 0)
            {
                speedUpGame(0);//Injection Rework, maybe that will useless, to be replaced with a function that allows game's refreshing
            }
        }
        //if (GetKeyState(VK_F1) & 0x00000001)
        //{
        //    printf("Player not playing !\n");
        //    Release_All_Inputs();
        //    generation->isPlaying = false;
        //}
        //else {
        //    //printf("Player is playing, WARNING !\n");
        //    generation->isPlaying = true;
        //}
        //render_frame();
    }
}

void render_frame()
{
    preseau = generation->m_joueurs[generation->joueur_actuel].m_reseau;
    float color = 1;
    std::thread* threads = new std::thread[preseau->layers_length];
    for (int i = 0; i < preseau->layers_length; i++)
    {
        threads[i] = std::thread(render_frameSub);
    }
    for (int i = 0; i < preseau->m_layerSizes[preseau->layers_length]; i++)
    {
        
        color = 0.5;
        window->draw_circle(Pos{ -700.0 + 75 * i,  200.0 + 105.0 * (preseau->layers_length) }, 30, Color{ 0.0, 0.0, color });
    }
    
    
    for (int i = 0; i < preseau->layers_length; i++)
    {
        threads[i].join();
    }
    window->update();
}

void render_frameSub()
{
    for (int j = 0; j < preseau->m_layerSizes[global_int]; j++)
    {
        //color = 0.2 + 0.8 * preseau->layers[i]->weights[j, preseau->m_layerSizes[i+1]];
        double color = 0.5;
        for (int k = 0; k < preseau->m_layerSizes[global_int + 1]; k++)
        {
            window->draw_line(Pos{ -700.0 + 75.0 * j, 200.0 + global_int * 105.0 }, Pos{ -700.0 + 75.0 * k, 200.0 + (global_int + 1) * 105.0 }, Color{ color, color, color });
        }
        color = 1;
        window->draw_circle(Pos{ -700.0 + 75.0 * j, 200.0 + (global_int * 105.0) }, 30, Color{ color, color, color });

    }
}