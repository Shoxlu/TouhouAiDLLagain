// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "utils.h"
#include <iostream>
#include "GenerationPerso.h"
#include "NeuralNetwork.h"
#include "Joueur.h"
#include "InputHelper.h"
#include "dllmain.h"
#include "Window.h"
#include "Drawer.h"

#include <GLFW/glfw3.h>
#include <thread>


//Touhou 18 global variables
zGlobals* global_ptr = (zGlobals*)0x4cccc0;
zPlayer* player_ptr = *(zPlayer**)0x4CF410;
zBulletManager* Bullet_PTR = *(zBulletManager**)0x4CF2BC;
int32_t* Inputs = (int32_t*)0x4CA210;
int32_t* Inputs_prev = (int32_t*)0x4CA214;
zPauseMenu* pauseMenu_ptr = *(zPauseMenu**)0x4CF40C;

BYTE frame_skip = 0;
GenerationHandler* generation;
InputHelper* pinputHelper;
bool isRendering;
constexpr int32_t NbrePerso_generation = 1000;
int32_t previous_time;
Window* window;
HANDLE hprocess;
HANDLE gameWindow;
NeuralNetwork* preseau;
Window_struct* WINDOW = (Window_struct*)0x568c30;




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
        //patch remaining BYTES;
        BYTE patch[] = { 0x90 };
        BYTE patch1[] = { 0x90, 0x90, 0x90 };
        BYTE patchInputs[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};// 0x89, 0x35, 0x10, 0xA2, 0x4C, 0x00 
        writeMemory(0x401E19, patchInputs, sizeof(patchInputs));
        writeMemory(0x401E0E, patchInputs, sizeof(patchInputs));
        writeMemory(0x4712DE, patch, sizeof(patch));
        writeMemory(0x00471C2A + 0x5, patch1, sizeof(patch1));

        printf("End on init \n");
        
    }
    case DLL_PROCESS_DETACH:
        if(pinputHelper)
            delete pinputHelper;
        if(generation)
            delete generation;
        speedUpGame(0);
    }
    return TRUE;
}

void init()
{
    LoadLibraryW(L"opengl32.dll");
    Sleep(5000);
    pinputHelper = new InputHelper();
    generation = new GenerationHandler(NbrePerso_generation);
    isRendering = true;
    previous_time = 0;
    auto new_thread1 = CreateRemoteThread(
        hprocess,
        NULL,
        NULL,
        (LPTHREAD_START_ROUTINE)render,
        NULL,
        NULL,
        NULL);

}

void HandleNoGameRender() {
    if (WINDOW->Window_active == 0)
        return;
    if (GetKeyState(VK_F1) & 1 && isRendering)
    {
        BYTE patch[] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
        writeMemory(0x00471C07, patch, sizeof(patch));
        isRendering = false;
    }
    else if (!isRendering && !(GetKeyState(VK_F1) & 1)) {
        //printf("Player is playing, WARNING !\n");
        BYTE patch[] = { 0xE8, 0x44, 0x0F, 0x00, 0x00 };
        writeMemory(0x00471C07, patch, sizeof(patch));
        isRendering = true;
    }
}
void HandleSpeedUp() {
    if (WINDOW->Window_active == 0)
        return;
    if (GetKeyState(VK_BACK) & 1)
        speedUpGame(20);
    else if (GetKeyState(VK_BACK) == 0)
        speedUpGame(0);
}

void HandleHumanInputs() {

    if (WINDOW->Window_active == 0)
        return;
    //if (GetKeyState(VK_SPACE) & 1)
    //    *Inputs = 0;
    if (GetKeyState(VK_W) & 0x10000000)
        *Inputs |= Shoot;
    if (GetKeyState(VK_X) & 0x10000000)
        *Inputs |= Bomb;
    if (GetKeyState(VK_SHIFT) & 0x10000000)
        *Inputs |= Focus;
    if (GetKeyState(VK_ESCAPE) & 0x10000000)
        *Inputs |= Esc;
    if (GetKeyState(VK_R) & 0x10000000)
        *Inputs |= R;
    if (GetKeyState(VK_UP) & 0x10000000)
        *Inputs |= Up;
    if (GetKeyState(VK_DOWN) & 0x10000000)
        *Inputs |= Down;
    if (GetKeyState(VK_LEFT) & 0x10000000)
        *Inputs |= Left;
    if (GetKeyState(VK_RIGHT) & 0x10000000)
        *Inputs |= Right;
    if (GetKeyState(VK_EXECUTE) & 0x10000000)
        *Inputs |= Enter;
}


void update()
{
    if (!generation) {
        init();
    }
    player_ptr = *(zPlayer**)0x4CF410;
    Bullet_PTR = *(zBulletManager**)0x4CF2BC;
    global_ptr = (zGlobals*)0x4cccc0;
    HandleNoGameRender();

    if(*Inputs_prev != *Inputs)
        *Inputs_prev = *Inputs;
    *Inputs = 0;
    if (player_ptr)
    {
        if (global_ptr->time_in_stage > previous_time)
        {
            previous_time = global_ptr->time_in_stage;
            generation->update();
            HandleSpeedUp();
        }
    }
    HandleHumanInputs();
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
        if (global_ptr->time_in_stage > previous_time && preseau != nullptr)
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