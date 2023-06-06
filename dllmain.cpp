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
#include "Drawer.h"

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
bool isRendering;
const int NbrePerso_generation = 4;
int previous_time;
Window* window;
HANDLE hprocess;
HANDLE gameWindow;
NeuralNetwork* preseau;
int actual_output;
void update();
void init();
void render();

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
        patch_call(0x004712D9, init);
        patch_call(0x00471C2A, update);
        BYTE patch[] = { 0x90 };
        BYTE patch1[] = { 0x90, 0x90, 0x90 };
        writeMemory(0x4712DE, patch, sizeof(patch));
        writeMemory(0x00471C2A + 0x5, patch1, sizeof(patch1));
        printf("End on init \n");
        
    }
    case DLL_PROCESS_DETACH:
        if(pinputHelper)
            delete pinputHelper;
        if(generation)
            //delete generation;
        speedUpGame(0);
        ReleaseAllInputs();
        press(VK_W, 1);
    }
    return TRUE;
}

void init()
{
    LoadLibraryW(L"opengl32.dll");
    srand(time(0));
    pinputHelper = new InputHelper();
    generation = new GenerationJoueur(NbrePerso_generation);
    isRendering = true;
    previous_time = 0;
    auto new_thread1 = CreateRemoteThread(
        hprocess,
        NULL,
        NULL,
        (LPTHREAD_START_ROUTINE)render,
        NULL,
        NULL,
        NULL
    );
}

void update()
{
    if (!generation) {
        init();
    }
    player_ptr = *(zPlayer**)0x4CF410;
    Bullet_PTR = *(zBulletManager**)0x4CF2BC;
    global_ptr = (zGlobals*)0x4cccc0;
    if (GetKeyState(VK_F1) & 0x00000001 && isRendering == true)
    {
        BYTE patch[] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
        writeMemory(0x00471C07, patch, sizeof(patch));
        isRendering = false;
    }
    else if (isRendering == false && !(GetKeyState(VK_F1) & 0x00000001)) {
        //printf("Player is playing, WARNING !\n");
        BYTE patch[] = { 0xE8, 0x44, 0x0F, 0x00, 0x00 };
        writeMemory(0x00471C07, patch, sizeof(patch));
        isRendering = true;
    }
    if (!player_ptr)
        return;
    if (global_ptr->time_in_stage > previous_time)
    {
        previous_time = global_ptr->time_in_stage;
        actual_output = generation->update();
        if (GetKeyState(VK_BACK) & 0x00000001)
            speedUpGame(10);
        else if (GetKeyState(VK_BACK) == 0)
            speedUpGame(0);
    }
}


void render()
{
    if (!window)
    {
        glfwInit();
        window = new Window();
    }
    Drawer drawer(nullptr, window);
    while (1) {
        if (global_ptr->time_in_stage > previous_time)
        {
            drawer.DrawNetwork(preseau);
            drawer.Apply();
        }
        else if(!player_ptr){
            Sleep(1);
            drawer.Apply();
        }
        

    }
}