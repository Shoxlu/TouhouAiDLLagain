
#include <random>
#include "List.h"
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

Pos get_player_pos()
{
    Pos playerPos;
    playerPos.x = 0;
    playerPos.y = 0;
    if (player_ptr == NULL)
    {
        printf("Player ptr not initialised !\n");
        return playerPos;
    }
    playerPos.x = player_ptr->inner.pos.x;
    playerPos.y = player_ptr->inner.pos.y;
    return playerPos;
    return Pos{ 0, 0 };
}
int bulletNear(float x, float y) {
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
        float bullet_hitbox = bullet->hitbox_radius;
        float plHitbox_radius = 10;
        float distance = pow(x - bullet->pos.x, 2) + pow(y - bullet->pos.y, 2);
        if (distance >= ((plHitbox_radius * plHitbox_radius) + bullet_hitbox * bullet_hitbox)) {
            float v12 = bullet_hitbox / 2.5;
            if (v12 < 10.0) {
                v12 = 10.0;
            }
            if (distance <= ((v12 + plHitbox_radius) * (v12 + plHitbox_radius)) + bullet_hitbox * bullet_hitbox) {
                return 1;
            }
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

void press(int input, bool release)
{
    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = input;
    if (release) {
        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = input;
        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
    }
    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}

float random_float() {
    return ((float)(rand()) / (float)(RAND_MAX)-0.5) * 2;
}
float random_float_positive() {
    return ((float)(rand()) / (float)(RAND_MAX));
}
int randint(int min, int max) {
    return min + rand() % ((max-min)+1);
}
bool random_bool()
{
    float temp = random_float_positive() * 99;
    if (temp > 50)
    {
        return true;
    }
    return false;
}
template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = NULL;
    }
}

void Release_All_Inputs()
{
    press(VK_W, 1);
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

