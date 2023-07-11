#pragma once
#define _USE_MATH_DEFINES 
#define _CRT_RAND_S  
#include <Windows.h>
#include <vector>
#include <math.h>

class GenerationHandler;
class InputHelper;
class NeuralNetwork;
struct Connection;
class Joueur;
class Window;
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
    int32_t id;
};
struct zTimer
{
    int32_t previous;
    int32_t current;
    float current_f;
    char __game_speed__disused;
    int32_t control;
};
struct zBulletNullList
{
    int32_t __unused_null_ptr;
    int32_t next;
    int32_t prev;
    int32_t __seldom_used;
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
    int32_t end_time;
    int32_t method;
};
struct zInterpFloat3
{
    zFloat3 initial;
    zFloat3 goal;
    zFloat3 bezier_1;
    zFloat3 bezier_2;
    zFloat3 current;
    zTimer time;
    int32_t end_time;
    int32_t method;
};
struct zBulletList
{
    int32_t entry;
    int32_t next;
    int32_t prev;
    int32_t __seldom_used;
};
struct zBullet
{
    zBulletNullList freelist_node;
    zBulletList node_10;
    int32_t flags;
    int32_t ex_invuln__remaining_frames;
    BYTE gap28[1544];
    char __unknown_1588;
    zFloat3 pos;
    zFloat3 velocity;
    float speed;
    float angle;
    float hitbox_radius;
    float __hitbox_radius__copy;
    int32_t index_in_bullet_array;
    int32_t ex_func_a;
    char __unknown_1640;
    __declspec(align(8)) int32_t __field_670__had_5;
    int32_t cancel_script_id;
    char __unknown_1656;
    int32_t ex_index;
    int32_t ex_loop__loop_count;
    int32_t active_ex_flags;
    int32_t initial_active_ex_flags;
    int32_t next_bullet_in_layer;
    char __unknown_1680;
    int32_t transform_sound;
    int32_t layer;
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
    int32_t bullet_def_ptr;
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
    int32_t x;
    int32_t y;
};
struct zPlayerInner
{
    zFloat3 pos;
    zInt2 internal_pos;
    BYTE gap14[60];
    zPlayerOption options[4];
    zPlayerOption equipment[12];
    zPlayerBullet bullets[512];
    int32_t last_created_damage_source_index;
    zPlayerDamageSource damage_sources[1025];
    char __unknown_290800;
    BYTE gap46FF1[155];
    int32_t state;
    char __unknown_290960;
    BYTE gap47091[31];
    zTimer shoot_key_short_timer;
    zTimer shoot_key_long_timer;
    char __unknown_291032;
    BYTE gap470D9[527];
    int32_t num_deathbomb_frames;
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
    int32_t on_tick;
    int32_t on_draw;
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
    int32_t __counter__does_something_every_4_cancels;
    int32_t __unused;
    int32_t iter_current;
    int32_t iter_current_2;
    int32_t iter_next;
    int32_t iter_next_2;
    int32_t __some_cancel_related_counter;
    int32_t bullet_anm;
    int32_t __flag_that_enables_unknown_counter;
    int32_t __unknown_counter__something_happens_every_6;
};

struct Window_struct
{
    HWND WINDOW;
    HWND hDlg;
    int32_t field_8;
    HINSTANCE hinst;
    int32_t Window_active;
    int32_t field_14;
    int32_t field_18;
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
    int32_t field_2050;
    int32_t cy;
    int32_t window_width;
    int32_t window_height;
    int32_t field_2060;
    int32_t field_2064;
    int32_t field_2068;
    int32_t field_206C;
    int32_t field_2070;
    int32_t field_2074;
    int32_t field_2078;
    int32_t playable_height;
    int32_t playable_width;
    int32_t field_2084;
    int32_t field_2088;
    int32_t field_208C;
    int32_t field_2090;
    char field_2094[4];
    double current_time;
    double last_time;
    double previous_global_time;
    double actual_time_second__;
    double actual_time;
    double actual_time__;
    int32_t field_20C8;
    int32_t field_20CC;
    int32_t game_state;
    int32_t field_20D4[12];
};
struct zEnemyList {
    zEnemy* entry;
    zEnemyList* next;
    zEnemyList* prev;
    int32_t __seldom_used;
};

struct zEnemyManager {
    int32_t vtable;
    int32_t on_tick;
    int32_t on_draw;
    int32_t ecl_global_int_vars[4];
    BYTE gap1C[32];
    int32_t miss_count;
    int32_t bomb_count;
    int32_t can_still_capture_spell;
    int32_t boss_ids[4];
    char __unknown_88;
    BYTE gap59[51];
    int32_t enemy_limit;
    int32_t next_enemy_id;
    int32_t last_enemy_id;
    char __unknown_152;
    BYTE gap99[243];
    zEnemyList* active_enemy_list_head;
    zEnemyList* active_enemy_list_tail;
    char __unknown_404;
    int32_t enemy_count_real;
    char __unknown_412;
};
struct zEclStack {
    int32_t data[1024];
    int32_t stack_offset;
    int32_t base_offset;
};

struct zEclLocation {
    int32_t subroutine_index;
    int32_t offset_from_first_instruction;
};

struct zEclRunContext {
    float time;
    zEclLocation cur_location;
    zEclStack stack;
    int32_t async_id;
    int32_t enemy;
    char __set_by_ins_20;
    char __unknown_4125;
    __declspec(align(4)) char difficulty_mask;
    char __unknown_4129;
    zInterpFloat float_i[8];
    int32_t float_i_locals_offsets_from_stack[8];
    zEclLocation float_i_locs[8];
    int32_t __set_by_ins_18_19;
};
struct zEclRunContextHolder {
    int32_t current_context;
    zEclRunContext primary_context;
};

struct zEclVm {
    int32_t vtable; 
    int32_t __next_in_some_list; 
    int32_t __prev_in_some_list; 
    zEclRunContextHolder context; 
    int32_t file_manager;
    int32_t enemy;
    int32_t async_list_head; 
    int32_t __unused__prev_ptr_of_list_head; 
    int32_t __unused__field_c_ptr_of_list_head;
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
    int32_t flags;
};

struct zFloat2 {
    float x;
    float y;
};

struct zInterpStrange1 {
    zFloat3 current;
    zFloat3 initial;
    zFloat3 goal;
    zFloat3 bezier_1;
    zFloat3 bezier_2;
    zTimer time;
    int end_time;
    int method_for_1d;
    int __field_58;
    int __field_5c;
    int method_for_3d;
    int flags;
};
struct zInterpFloat2 {
    zFloat2 initial;
    zFloat2 goal;
    zFloat2 bezier_1;
    zFloat2 bezier_2;
    zFloat2 current;
    zTimer time;
    int end_time;
    int method;
};

struct zBulletEx {
    float r;
    float s;
    float m;
    float n;
    int a;
    int b;
    int c;
    int d;
    int type;
    int async;
    int string;
};

struct zEnemyLife {
    int current;
    int maximum;
    int remaining_for_cur_attack;
    int current_scaled_by_seven;
    int starting_value_for_next_attack;
    int total_damage_including_ignored;
    int is_spell;
};

struct zEnemyBulletShooter {
    int type;
    int color;
    zFloat3 offset_final;
    float angle_1;
    float angle_2;
    float speed_1;
    float speed_2;
    float distance;
    zBulletEx ex[24];
    char __unknown_1096;
    BYTE gap449[15];
    int laser_time_start;
    int laser_trans_1;
    int laser_duration;
    int laser_trans_2;
    int laser_time_end;
    __int16 count_1;
    __int16 count_2;
    int aim_type;
    int sound_flag;
    int shot_sound;
    int shot_transform_sound;
    int start_transform;
    int __field_484;
};
struct zBulletOffset {
    zFloat2 xy;
    float __unknown;
};

struct zEnemyDrop { int main_drop; int ex_drop[20]; int timed_ex_drop[20]; int timed_ex_drop_time; zTimer timed_ex_drop_timer; float width; float height; };

struct zEnemyInterrupt { int hp_value; int time; char __sub_name_1[64]; char __sub_name_2[64]; };
struct zEnemyFog { int fog_ptr; char __unknown_4; float fog_radius; float __fog_field_c__init_16f; int fog_color; float __fog_angle_44d0; float __fog_angle_44d4; };

struct zEnemyData {
    zPosVel prev_final_pos;
    zPosVel final_pos;
    zPosVel abs_pos;
    zPosVel rel_pos;
    zFloat2 hurtbox_size; 
    zFloat2 hitbox_size;
    float hurtbox_rotation; 
    zAnmId anm_vm_ids[16];
    zFloat3 anm_pos[16]; 
    zAnmId __field_224_lolidfk[16];
    int selected_anm_index; 
    int anm_slot_0_anm_index; 
    int anm_slot_0_script;
    int anm_main;
    int __probably_cur_subscript_for_anm_main;
    int kill_id; int anm_layers;
    zFloat3 __pos_of_last_damage_source_to_hit_me__butwhytho;
    int ecl_int_vars[4]; float ecl_float_vars[8];
    zTimer time_in_ecl; 
    zTimer time_alive;
    float slowdown; 
    zEnemyList node_in_global_storage; 
    zInterpStrange1 abs_pos_i; 
    zInterpStrange1 rel_pos_i; 
    zInterpFloat abs_angle_i; 
    zInterpFloat abs_speed_i;
    zInterpFloat rel_angle_i; 
    zInterpFloat rel_speed_i; 
    zInterpFloat2 abs_radial_dist_i; 
    zInterpFloat2 rel_radial_dist_i;
    zInterpFloat2 abs_ellipse_i; 
    zInterpFloat2 rel_ellipse_i; 
    zEnemyBulletShooter bullet_props[16]; 
    int et_ex_index[16]; 
    zBulletOffset bullet_mgr_offsets[16]; 
    zBulletOffset bullet_mgr_origins[16]; 
    zFloat2 final_sprite_size; 
    zFloat2 move_limit_center; 
    zFloat2 move_limit_size; 
    int score_reward;
    zEnemyLife life; 
    zEnemyDrop drop; 
    int __field_50d4__some_kind_of_extra_damage;
    int death_sound; 
    int death_anm_script; 
    int death_anm_index;
    int __frames_before_next_hurt_fx__i_think; 
    int __i_think_this_field_is_legit_unused; 
    int hit_sound;
    zTimer invuln_timer;
    zTimer no_hitbox_timer;
    zTimer __timer_5118__counts_down; 
    float bomb_damage_multiplier;
    int flags_lo; 
    int flags_hi; 
    int bombshield_on_anm_main;
    int bombshield_off_anm_main;
    int own_boss_id; 
    float et_protect_range; 
    zEnemyInterrupt interrupts[8]; 
    int full; 
    zEnemyFog fog; 
    char set_death[64]; 
    int func_from_ecl_func_set; 
    int is_func_set_2; 
    int func_from_ecl_func_ext_dmg;
    int func_from_ecl_hitbox_func; 
    int own_chapter; 
    int __bool_cleared_by_ecl_570;
};

struct zEnemy {
    zEclVm ecl; 
    zEnemyData data; 
    int32_t on_death_callback;
    int32_t enemy_id;
    int32_t parent_enemy_id; 
    int32_t __only_used_in_ddc;
};
struct Output {
    double speed;
    double angle;
};
struct Dir {
    int32_t key;
};
struct zGlobalsInner {
    int32_t stage_num;
    int32_t gap[12];
    int32_t miss_count;
};
struct zGlobals {
    BYTE gap[28];//0x0
    int32_t stage_num;//28
    int32_t stage_num_started_from;//?????????? //32
    BYTE gap2[4];//36
    int32_t time_in_stage;//40
    BYTE gap1[30];//44
    int32_t graze_counter;//74
    BYTE gap4[10];//78
    int32_t miss_count;//88
    BYTE gap3[38];//92
    int32_t current_lives;//130 ???
    BYTE gap10[12];
    int32_t current_bombs;
    //offset 0x88 : Global life count
};
struct Color {
    double r;
    double g;
    double b;
    double a;
};
struct DataPoint32_t {
    double inputs[2];
    double expectedOutputs[2];
};

struct zMenuSelect {
    int32_t selection_index;
    int32_t selection_on_prev_tick;
    int32_t selection_count;
    int32_t selection_stack[16];
    int32_t selection_count_stack[16];
    int32_t current_stack_index;
    int32_t disabled_selections[16];
    int32_t wrap_around;
    int32_t num_disabled_selections;
};

struct zPauseMenu {
    BYTE gap0[16];
    int32_t time_in_menu;
    BYTE gap14[32];
    zMenuSelect another_menu__pause;
    zMenuSelect char_select_menu;
    int32_t anmrelated;//actually AnmManger ptr i think
    BYTE gap1E8[12];
    int32_t menu_type;
    int32_t string_pos;
    BYTE gap1FC[216];
    char name_to_enter[8];
    BYTE gap2DC[12];
    int32_t field_0;
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

struct Node {
    size_t id;
    size_t type;
    double value;
    std::vector<Connection> incoming_connections;
    bool already_visited; //for topoligical sorting
};

struct Connection {
    size_t Inid;
    size_t OutId;
    double weight;
    bool state;//To tell if the connection has to be used (because overlapping connection are disabled)
    size_t InnovId; //actually i don't know how it's used;
};


struct Specie {
    std::vector<Joueur*> networks;
    int32_t avgReward = 0;
    int32_t maxReward = 0;
    int32_t totalReward = 0;
    size_t nbChildren = 0;
};

struct Pos {
    double x;
    double y;
};



#define VK_W 0x57
#define VK_R 0x52
#define VK_D 0x44
#define VK_C 0x43
#define VK_X 0x58


//Game's data related
static constexpr int32_t NINPUTSPBULLET = 7;
static constexpr int32_t NINPUTSENEMY = 4;
static constexpr auto N_BULLETS = 2000;
static constexpr auto INPUTS_MAX = NINPUTSENEMY + NINPUTSPBULLET + 2;
static constexpr auto OUTPUTS_MAX = 5;

//Nodes types
static constexpr size_t SENSOR = 0;
static constexpr size_t HIDDEN = 1;
static constexpr size_t OUTPUT = 2;

//Window things
static constexpr auto WIDTH = 853;
static constexpr auto HEIGHT = 613;

extern zGlobals* global_ptr;
extern zPlayer* player_ptr;
extern zBulletManager* Bullet_PTR;
extern int32_t* Inputs;
extern int32_t* MenuInputs;
extern int32_t* Inputs_prev;
extern int32_t* MenuInputs_prev;
extern zPauseMenu* pauseMenu_ptr;

extern BYTE frame_skip;
extern GenerationHandler* generation;
extern InputHelper* pinputHelper;
extern bool isRendering;
extern int32_t previous_time;
extern Window* window;
extern HANDLE hprocess;
extern HANDLE gameWindow;
extern NeuralNetwork* preseau;
extern Window_struct* WINDOW;


void press(int32_t input, bool release);
double random_float();
double random_float_positive();
int32_t randint(int32_t min, int32_t max);
bool random_bool();
Pos get_player_pos();
void ReleaseAllInputs();
size_t GetMaximumIndex(double outputs[], size_t length);
size_t GetMaximumIndex(std::vector<double> outputs, size_t length);
double ActivationFunction(double weightedInput);
void speedUpGame(int32_t speed);
void writeMemory(void* dst, void* src, size_t len);
void writeMemory(DWORD dst, void* src, size_t len);
void patch_call(void* target, void* func);
void patch_call(DWORD target, void* func);
size_t getMaxInnovId(std::vector<Connection> connections);