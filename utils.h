#pragma once
#include <Windows.h>
#include <vector>

struct zBullet;
struct zEnemy;
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
    BYTE gap28[1544];
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
    BYTE bytes[4];
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

struct Window_struct
{
    HWND WINDOW;
    HWND hDlg;
    int field_8;
    HINSTANCE hinst;
    int Window_active;
    int field_14;
    int field_18;
    char field_1C;
    char field_1D[3];
    LARGE_INTEGER Frequency;
    LARGE_INTEGER PerformanceCount;
    char field_30;
    char APPDAT_Path[4096];
    char EXE_PATH[4099];
    UINT screen_saver_active;
    UINT screen_saver_low_power_active;
    UINT screen_saver_power_off_active;
    char field_2040;
    __declspec(align(4)) uint32_t __counter_2044;
    char field_2048[8];
    int field_2050;
    int cy;
    int window_width;
    int window_height;
    int field_2060;
    int field_2064;
    int field_2068;
    int field_206C;
    int field_2070;
    int field_2074;
    int field_2078;
    int playable_height;
    int playable_width;
    int field_2084;
    int field_2088;
    int field_208C;
    int field_2090;
    char field_2094[4];
    double current_time;
    double last_time;
    double previous_global_time;
    double actual_time_second__;
    double actual_time;
    double actual_time__;
    int field_20C8;
    int field_20CC;
    int game_state;
    int field_20D4[12];
};
struct zEnemyList {
    zEnemy* entry;
    zEnemyList* next;
    zEnemyList* prev;
    int __seldom_used;
};

struct zEnemyManager {
    int vtable;
    int on_tick;
    int on_draw;
    int ecl_global_int_vars[4];
    BYTE gap1C[32];
    int miss_count;
    int bomb_count;
    int can_still_capture_spell;
    int boss_ids[4];
    char __unknown_88;
    BYTE gap59[51];
    int enemy_limit;
    int next_enemy_id;
    int last_enemy_id;
    char __unknown_152;
    BYTE gap99[243];
    zEnemyList* active_enemy_list_head;
    zEnemyList* active_enemy_list_tail;
    char __unknown_404;
    int enemy_count_real;
    char __unknown_412;
};
struct zEclStack {
    int data[1024];
    int stack_offset;
    int base_offset;
};

struct zEclLocation {
    int subroutine_index;
    int offset_from_first_instruction;
};

struct zEclRunContext {
    float time;
    zEclLocation cur_location;
    zEclStack stack;
    int async_id;
    int enemy;
    char __set_by_ins_20;
    char __unknown_4125;
    __declspec(align(4)) char difficulty_mask;
    char __unknown_4129;
    zInterpFloat float_i[8];
    int float_i_locals_offsets_from_stack[8];
    zEclLocation float_i_locs[8];
    int __set_by_ins_18_19;
};
struct zEclRunContextHolder {
    int current_context;
    zEclRunContext primary_context;
};

struct zEclVm {
    int vtable; 
    int __next_in_some_list; 
    int __prev_in_some_list; 
    zEclRunContextHolder context; 
    int file_manager;
    int enemy;
    int async_list_head; 
    int __unused__prev_ptr_of_list_head; 
    int __unused__field_c_ptr_of_list_head;
};
struct zPosVel {
    zFloat3 pos;
    zFloat3 __another_pos;
    float vel_norm__or__circle_angular_velocity;
    float angle; float circle_radius;
    float circle_radial_speed;
    float __some_angle;
    char __unknown_44;
    BYTE gap2D[7];
    zFloat3 __field_34__maybe_velocity__idfk;
    int flags;
};
struct zEnemyData {
    zPosVel prev_final_pos;
    zPosVel final_pos;
    zPosVel abs_pos;
    zPosVel rel_pos;
    //i deleted fields from here
};

struct zEnemy {
    zEclVm ecl; 
    zEnemyData data; 
    int on_death_callback;
    int enemy_id;
    int parent_enemy_id; 
    int __only_used_in_ddc;
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
    int key;
};
struct zGlobalsInner {
    int stage_num;
    int gap[12];
    int miss_count;
};
struct zGlobals {

    BYTE gap[32];//0x0
    int stage_num;//32
    BYTE gap2[4];//36
    int time_in_stage;//42
    BYTE gap1[44];//46
    int miss_count;//90
    BYTE gap3[42];//94
    int current_lives;//136
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

struct zMenuSelect {
    int selection_index;
    int selection_on_prev_tick;
    int selection_count;
    int selection_stack[16];
    int selection_count_stack[16];
    int current_stack_index;
    int disabled_selections[16];
    int wrap_around;
    int num_disabled_selections;
};

struct zPauseMenu {
    BYTE gap0[16];
    int time_in_menu;
    BYTE gap14[32];
    zMenuSelect another_menu__pause;
    zMenuSelect char_select_menu;
    int anmrelated;//actually AnmManger ptr i think
    BYTE gap1E8[12];
    int menu_type;
    int string_pos;
    BYTE gap1FC[216];
    char name_to_enter[8];
    BYTE gap2DC[12];
    int field_0;
};
enum Keys {
    Shoot = 1,
    Bomb = 2,
    Focus = 8,
    Up = 0x10,
    Down = 0x20,
    Left = 0x40,
    Right = 0x80,
    Esc = 0x100,
    R = 0x200000,
    Enter = 0x80000
};



void press(int input, bool release);
double random_float();
double random_float_positive();
int randint(int min, int max);
bool random_bool();
Pos get_player_pos();
void input_number(uint32_t id);
void ReleaseAllInputs();
int GetMaximumIndex(double outputs[], int length);
int GetMaximumIndex(std::vector<double> outputs, int length);
double ActivationFunction(double weightedInput);
void speedUpGame(int speed);
void writeMemory(void* dst, void* src, size_t len);
void writeMemory(DWORD dst, void* src, size_t len);
void patch_call(void* target, void* func);
void patch_call(DWORD target, void* func);