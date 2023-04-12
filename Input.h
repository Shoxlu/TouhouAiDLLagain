
#include "utils.h"
#include "GenericNeurone.h"
class Joueur;
#define isEnemy 0


class Input: public GenericNeurone
{
public:
    Input(double x, double y, int n_hidden_neurones_1, Joueur* joueur);
    Input(double x, double y, int n_hidden_neurones_1,float* weights, Joueur* joueur);
    ~Input();
    int get_type() const;
    Pos get_pos() const;
    Pos get_offset() const;
    float get_input() const;
    virtual float get_output(int k, int self_id);
    void Update();
    virtual bool isActive();
    void move();
    float* get_weights();
    float check_state();
    void mutation_Input();
    virtual void change_weights(int n_inputs);
    virtual int get_n_outputs();
    virtual void change_input(GenericNeurone** Inputs, int n_inputs);

private:
    Pos m_pos;
    Pos m_offset;
    int m_type;
    Joueur* m_joueur;
    float m_output;
    float m_input;
    int m_isActive;
    float* m_weights;
    int m_n_outputs;
};