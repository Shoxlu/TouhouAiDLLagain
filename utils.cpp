
#include "utils.h"

Pos get_player_pos()
{
    Pos playerPos = Pos{ 0, 0 };
    if (player_ptr == NULL)
    {
        printf("Player ptr not initialised !\n");
        return playerPos;
    }
    playerPos.x = player_ptr->inner.pos.x;
    playerPos.y = player_ptr->inner.pos.y;
    return playerPos;
}

void speedUpGame(int speed)
{
    BYTE* frameskip = (BYTE*)0x4CD00C;
    *frameskip = speed;
    frame_skip = *frameskip;
}
void press(int input, bool release)
{
    INPUT inputs[1] = {};
    ZeroMemory(inputs, sizeof(inputs));
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = input;
    if (release) {
        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
    }
    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}
//actually double
double random_float() {
    return ((double)(rand()) / (double)(RAND_MAX)-0.5) * 2;
}
//actually double
double random_float_positive() {
    return ((double)(rand()) / (double)(RAND_MAX));
}
int randint(int min, int max) {
    return min + rand() % ((max-min)+1);
}
bool random_bool()
{
    double temp = random_float_positive() * 99;
    if (temp > 50.0)
    {
        return true;
    }
    return false;
}

void ReleaseAllInputs()
{
    //press(VK_W, 1);
    press(VK_R, 1);
    press(VK_X, 1);
    press(VK_C, 1);
    press(VK_D, 1);
    press(VK_UP, 1);
    press(VK_DOWN, 1);
    press(VK_RIGHT, 1);
    press(VK_LEFT, 1);
    press(VK_SHIFT, 1);
}

void copy_weights(int n, float* dest, float* source)
{
    for (int i = 0; i < n; i++) {
        dest[i] = source[i];
    }
}

double ActivationFunction(double weightedInput) {
    /*printf("%f \n", weightedInput);
    printf("%f \n", 1.0 / (1.0 + exp(-weightedInput)));*/
    return 1.0 / (1.0 + exp(-weightedInput));
}
//returns the index of the maximum value of an array;
int GetMaximumIndex(double outputs[], int length) {
    int maximum_index = 0;
    double maximum = outputs[0];
    for (int i = 1; i < length; i++) {
        double output = outputs[i];
        if (maximum <= outputs[i])
        {
            maximum_index = i;
            maximum = outputs[i];
        }
    }
    return maximum_index;
}

int GetMaximumIndex(std::vector<double> outputs, int length) {
    int maximum_index = 0;
    double maximum = outputs[0];
    for (int i = 1; i < length; i++) {
        double output = outputs[i];
        if (maximum <= outputs[i])
        {
            maximum_index = i;
            maximum = outputs[i];
        }
    }
    return maximum_index;
}

void writeMemory(void* dst, void* src, size_t len) {
    DWORD prot;
    VirtualProtect(dst, len, PAGE_READWRITE, &prot);
    memcpy(dst, src, len);
    VirtualProtect(dst, len, prot, &prot);
}
void writeMemory(DWORD dst, void* src, size_t len) {
    writeMemory((void*)dst, src, len);
}

void patch_call(void* target, void* func) {
    BYTE patch[5];
    patch[0] = 0xE8;
    *(DWORD*)(patch + 1) = (DWORD)func - (DWORD)target - 5;
    writeMemory(target, patch, sizeof(patch));
}

void patch_call(DWORD target, void* func) {
    patch_call((void*)target, func);
}
