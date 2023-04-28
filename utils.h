#pragma once
#include <Windows.h>


struct zBullet;
struct zBulletList;
struct zPlayer;
struct zBulletManager;
struct zTimer;
struct zAnmId;
struct zAnmVm;
struct zFloat3;
struct zBulletListNull;
struct zGlobalsInner;
struct zGlobals;

struct zAnmId
{
    int id;
};
struct zTimer
{
    int previous;
    int current;
    float current_f;
    char __game_speed__disused;
    int control;
};
struct zBulletNullList
{
    int __unused_null_ptr;
    int next;
    int prev;
    int __seldom_used;
};
struct zFloat3
{
    float x;
    float y;
    float z;
};
struct zInterpFloat
{
    float initial;
    float goal;
    float bezier_1;
    float bezier_2;
    float current;
    zTimer time;
    int end_time;
    int method;
};
struct zInterpFloat3
{
    zFloat3 initial;
    zFloat3 goal;
    zFloat3 bezier_1;
    zFloat3 bezier_2;
    zFloat3 current;
    zTimer time;
    int end_time;
    int method;
};
struct zBulletList
{
    int entry;
    int next;
    int prev;
    int __seldom_used;
};
struct zBullet
{
    zBulletNullList freelist_node;
    zBulletList node_10;
    int flags;
    int ex_invuln__remaining_frames;
    BYTE gap28[1548];
    char __unknown_1588;
    zFloat3 pos;
    zFloat3 velocity;
    float speed;
    float angle;
    float hitbox_radius;
    float __hitbox_radius__copy;
    int index_in_bullet_array;
    int ex_func_a;
    char __unknown_1640;
    __declspec(align(8)) int __field_670__had_5;
    int cancel_script_id;
    char __unknown_1656;
    int ex_index;
    int ex_loop__loop_count;
    int active_ex_flags;
    int initial_active_ex_flags;
    int next_bullet_in_layer;
    char __unknown_1680;
    int transform_sound;
    int layer;
    BYTE gap69C[2064];
    char __unknown_3756;
    zInterpFloat3 ex_move_i;
    zInterpFloat scale_i;
    float scale;
    zTimer __timer_f3c;
    zTimer __timer_f50;
    char __unknown_3940;
    __declspec(align(4)) __int16 state;
    char __unknown_3946;
    zTimer __timer_f6c;
    zTimer __timer_f80;
    int bullet_def_ptr;
    __int16 type;
    __int16 color;
    char __unknown_3996;
};
struct zPlayerBullet
{
    char __unknown_0;
    BYTE gap1[247];
};
struct zPlayerDamageSource
{
    char __unknown_0;
    BYTE gap1[155];
};
struct zPlayerOption
{
    char __unknown_0;
    BYTE gap1[239];
};
struct zInt2
{
    int x;
    int y;
};
struct zPlayerInner
{
    zFloat3 pos;
    zInt2 internal_pos;
    BYTE gap14[60];
    zPlayerOption options[4];
    zPlayerOption equipment[12];
    zPlayerBullet bullets[512];
    int last_created_damage_source_index;
    zPlayerDamageSource damage_sources[1025];
    char __unknown_290800;
    BYTE gap46FF1[155];
    int state;
    char __unknown_290960;
    BYTE gap47091[31];
    zTimer shoot_key_short_timer;
    zTimer shoot_key_long_timer;
    char __unknown_291032;
    BYTE gap470D9[527];
    int num_deathbomb_frames;
    char __unknown_291564;
    BYTE gap472ED[23];
};
struct zPlayer
{
    char __unknown_0;
    BYTE gap1[1567];
    zPlayerInner inner;
    char __unknown_293156;
    BYTE gap47925[39];
    zInterpFloat player_scale_i;
    float player_scale__requires_flag_0x10__from_ddc;
    float damage_multiplier;
    char __unknown_293252;
    float __might_be_attract_speed;
    float item_collect_radius;
    float item_attraction_radius_focused;
    float item_attraction_radius_unfocused;
    float poc_height;
    float __field_4799c__init_3f;
    float __field_479a0__init_3f;
    float __field_479a4__init_1f;
    float __field_479a8__init_1f;
    float __field_479ac__init_5f;
    float __field_479b0__init_1f;
    float __field_479b4__init_1f;
    float __field_479b8__init_5f;
    char __unknown_293308;
    BYTE gap479BD[23];
};
struct zBulletManager
{
    char __unknown_0;
    int on_tick;
    int on_draw;
    char __unknown_12;
    BYTE gapD[55];
    float et_protect_range;
    float __field_48;
    float __field_4c;
    char __unknown_80;
    BYTE gap51[107];
    zBulletList tick_list_head;
    char __unknown_204;
    BYTE gapCD[31];
    zBullet list_0_tail_dummy_bullet;
    zBullet bullets[2000];
    zAnmId anm_ids[2001];
    int __counter__does_something_every_4_cancels;
    int __unused;
    int iter_current;
    int iter_current_2;
    int iter_next;
    int iter_next_2;
    int __some_cancel_related_counter;
    int bullet_anm;
    int __flag_that_enables_unknown_counter;
    int __unknown_counter__something_happens_every_6;
};

struct Pos {
    double x;
    double y;
};
struct Output {
    double speed;
    double angle;
};
struct Dir {
    int dir[2];
    int n_dir;
    bool released;
};
struct zGlobalsInner {
    int stage_num;
    int gap[12];
    int miss_count;
};
struct zGlobals {

    BYTE gap[32];
    int stage_num;
    BYTE gap2[4];
    int time_in_stage;
    BYTE gap1[44];
    int miss_count;
    //offset 0x88 : Global life count
};
struct Color {
    double r;
    double g;
    double b;
    double a;
};
struct DataPoint {
    double inputs[2];
    double expectedOutputs[2];
};
void press(Dir input, bool release);
void press(int input, bool release);
double random_float();
double random_float_positive();
int randint(int min, int max);
bool random_bool();
Pos get_player_pos();
int bulletNear(float x, float y);
void Release_All_Inputs();
void copy_array(int n, double* dest, double* source);
void copy_array(int n, float* dest, float* source);
void copy_array(int n, int* dest, int* source);
int GetMaximumIndex(double outputs[], int length);
double ActivationFunction(double weightedInput);
double ActivationFunctionDerivative(double x);
void speedUpGame(int speed);
void writeMemory(void* dst, void* src, size_t len);
void writeMemory(DWORD dst, void* src, size_t len);
void patch_call(void* target, void* func);
void patch_call(DWORD target, void* func);