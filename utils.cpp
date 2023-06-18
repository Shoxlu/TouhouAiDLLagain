
#include <random>
#include "utils.h"


#define PLAYER_PTR 
#define BULLET_MANAGER_PTR
#define GUI_PTR (uintptr_t*)0x4CF2E0
#define GLOBAL_PTR 
#define VK_W 0x57
#define VK_R 0x52
#define VK_D 0x44
#define VK_C 0x43
#define VK_X 0x58

zPlayer* player_ptr = *(zPlayer**)0x4CF410;
zBulletManager* Bullet_PTR = *(zBulletManager**)0x4CF2BC;
zGlobals* global_ptr = (zGlobals*)0x4cccc0;
BYTE frame_skip = 0;


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

zBullet* bulletNear(double x, double y) {
    Bullet_PTR = *(zBulletManager**)0x4CF2BC;
    if (!Bullet_PTR)
    {
        return 0;
    }

    auto first_bullet = (zBulletList*)Bullet_PTR->tick_list_head.next;
    Bullet_PTR->iter_current = (int)first_bullet;
    if (!first_bullet)
    {
        Bullet_PTR->iter_next = 0;
        return 0;
    }

    Bullet_PTR->iter_next = first_bullet->next;
    zBullet* bullet = (zBullet*)first_bullet->entry;


    while (bullet) {
        double bullet_hitbox = bullet->hitbox_radius;
        double plHitbox_radius = 10;
        double distance = sqrt(pow(x - bullet->pos.x, 2) + pow(y - bullet->pos.y, 2));
        if (distance >= sqrt(((plHitbox_radius * plHitbox_radius) + bullet_hitbox * bullet_hitbox))) {
            double v12 = bullet_hitbox;
            if (v12 < 20.0) {
                v12 = 20.0;
            }
            if (distance <= (sqrt(pow((plHitbox_radius),2) + bullet_hitbox * bullet_hitbox))+ v12 ) {
                return bullet;
            }
            
        }
        else {
            press(VK_X, 0);
            Sleep(1);
            press(VK_X, 1);
        }
        zBulletList* iter_next = (zBulletList*)Bullet_PTR->iter_next;
        Bullet_PTR->iter_current = (int)iter_next;
        if (!iter_next) {
            break;
        }
        Bullet_PTR->iter_next = iter_next->next;
        bullet = (zBullet*)iter_next->entry;
    }
    return 0;
}

void press(Dir input, bool release)
{
    INPUT* inputs = new INPUT[input.n_dir];
    ZeroMemory(inputs, sizeof(inputs));
    for (int i = 0; i < input.n_dir; i++)
    {
        inputs[i].type = INPUT_KEYBOARD;
        inputs[i].ki.wVk = input.dir[i];
        if (release) {
            inputs[i].ki.dwFlags = KEYEVENTF_KEYUP;
        }
    }
    SendInput(input.n_dir, inputs, sizeof(INPUT));
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