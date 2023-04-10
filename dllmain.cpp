// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <libloaderapi.h>
#include <iostream>
#include <time.h>
#include <wincodec.h>
#include <Windows.h>
#include <string>
#include <d3d9.h>
#include <d2d1.h>


#include "utils.h"
#include "TestJeuAI.h"
#include "GenerationPerso.h"
#include "Personnage.h"
#include "Reseau.h"
#include "Hidden_neurone.h"
#include "Input.h"
#include "dllmain.h"
#include "Window.h"

#include <GLFW/glfw3.h>
//#include <thread>


#pragma warning(disable : 4996)

using namespace std;


#define VK_W 0x57
#define VK_R 0x52
#define VK_D 0x44
#define VK_C 0x43
#define VK_X 0x58

extern GenerationJoueur* generation;
extern Joueur** joueur;
extern zPlayer* player_ptr;
extern zPlayer* player_ptr;
extern zBulletManager* Bullet_PTR;
extern zGlobals* global_ptr;
GenerationJoueur* generation;
const int NbrePerso_generation = 10;
int previous_time;
Window* window;
void update();
void init();
void render_frame();

BOOL APIENTRY DllMain(HMODULE module, DWORD reasonForCall, LPVOID reserved)
{
    
    switch (reasonForCall)
    {
    case DLL_PROCESS_ATTACH:
    {
        AllocConsole();
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stdout);
        SetConsoleTitle(L"Th18 IA");
        printf("Preparing to run........\n");
        float bullet_x = 0;
        float bullet_y = 0;
        float bullet_speed = 0;

        Sleep(1000);

        printf("DLL loaded!\n");
        HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
        
        auto id = GetCurrentProcessId();
        auto hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
        //init();
        auto new_thread1 = CreateRemoteThread(
            hprocess,
            NULL,
            NULL,
            (LPTHREAD_START_ROUTINE)init,
            NULL,
            NULL,
            NULL
        );
        if (new_thread1 == NULL)
        {
            printf("Fail creating thread \n");
        }
        printf("End on init \n");
        
    }
    case DLL_PROCESS_DETACH:
        Release_All_Inputs();
    }
    return TRUE;
}

void init()
{
    LoadLibraryW(L"opengl32.dll");
    srand(time(0));
    if (!glfwInit()) {
        printf("fail\n");
    }
    if (window == NULL)
    {
        window = new Window();
    }
    generation = new GenerationJoueur(NbrePerso_generation);

    press(VK_W, 0);
    update();
}

void update()
{
    previous_time = 0;
    auto joueurs = generation->get_joueurs();
    while(1)
    {
        player_ptr = *(zPlayer**)0x4CF410;
        Bullet_PTR = *(zBulletManager**)0x4CF2BC;
        global_ptr = (zGlobals*)0x4cccc0;
        
        if (global_ptr->time_in_stage > previous_time)
        {
            if (player_ptr != NULL)
            {
                generation->update();
            }
            render_frame();
        }
        
    }
}

void render_frame()
{
    auto joueurs = generation->get_joueurs();
    auto reseau = joueurs[generation->get_joueur_actuel()]->get_reseau();
    int n_inputs = reseau->get_n_inputs();
    auto inputs = reseau->get_inputs();
    int n_hidden_layer = reseau->get_n_Hidden_layer();
    int n_hidden_neurones = reseau->get_n_Hidden_neurones();
    auto hidden_layer = reseau->get_Hidden_layers();
    Pos player_pos = get_player_pos();

    window->draw_circle(Pos{ player_pos.x * 2, 200 - player_pos.y * 1.5f }, 10, Color{ 1, 0, 0 });
    for (int j = 0; j < n_inputs; j++)
    {
        float color = 0.2 + 0.8 * inputs[j]->isActive();
        Pos pos = inputs[j]->get_pos();
        window->draw_circle(Pos{ pos.x * 2, 200 - pos.y * 1.5f }, 25, Color{ color, color, color });
        for (int j = 0; j < n_hidden_neurones; j++)
        {
            window->draw_line(Pos{ pos.x * 2, 200 - pos.y * 1.5f }, Pos{ -300.0 + 75 * j, 200.0});
            float color = 0.2 + 0.8 * hidden_layer[0][j]->get_output();
            window->draw_circle(Pos{ -300.0 + 75.0 * j, 200.0}, 25, Color{color, color, color});
        }
        
    }
    for (int i = 1; i < n_hidden_layer; i++)
    {
        for (int j = 0; j < n_hidden_neurones; j++)
        {
            window->draw_line(Pos{ -300.0 + 75 * (j-1), 200.0 + 75 * (i-1) }, Pos{ -300.0 + 75 * j, 200.0 + 55 * j });
            float color = 0.2 + 0.8 * hidden_layer[i][j]->get_output();
            window->draw_circle(Pos{ -300.0 + 75 * j, 200.0 + 75 * i}, 25, Color{color, color, color});
        }
    }
    auto output = reseau->get_outputs();
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < n_hidden_neurones; j++)
        {
            window->draw_line(Pos{ -300.0 + 75 * j, 200.0 + 75 * (n_hidden_layer - 1) }, Pos{ -300.0 + 75 * i, 400.0 });
        }
        float color = 0.2 + 0.8 * output[i]->get_output();
        window->draw_circle(Pos{ -300.0 + 75 * i, 400 }, 25, Color{ 0.0, 0.0, color });
    }
    window->update();

}
