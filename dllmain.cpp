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
const int NbrePerso_generation = 10;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:{
        // console
        AllocConsole();
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stdout);
        SetConsoleTitle(L"Th18 IA");
        printf("DLL loaded!\n");
        HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
        srand(time(0));
        //init_direct2d();
        generation = new GenerationJoueur(NbrePerso_generation);
        joueur = generation->get_joueurs();
 
        while (GetKeyState(VK_TAB) == 0) {
            Sleep(1);
        }

        printf("Preparing to run........\n");
        float bullet_x = 0;
        float bullet_y = 0;
        float bullet_speed = 0;

        Sleep(1000);
        printf("Running !\n");
        press(VK_W, 0);
        press(VK_SHIFT, 0);
        int previous_time = global_ptr->time_in_stage;
        while (GetKeyState(VK_F1) == 0) {
            //add frame limiter (somehow)
            player_ptr = *(zPlayer**)0x4CF410;
            Bullet_PTR = *(zBulletManager**)0x4CF2BC;
            global_ptr = (zGlobals*)0x4cccc0;
            
            if (global_ptr->time_in_stage > previous_time)
            {
                previous_time = global_ptr->time_in_stage;
                if (player_ptr != NULL)
                {
                    generation->update();
                }

            }
        }
        printf("end\n");
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        Release_All_Inputs();
        break;
    }
    return TRUE;
}



