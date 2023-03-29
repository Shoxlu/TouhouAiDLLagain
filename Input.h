
#include "utils.h"
#include "GenericNeurone.h"
class Joueur;
#define isEnemy 0


class Input: public GenericNeurone
{
public:
    Input(double x, double y, Joueur* joueur);
    Input(double x, double y, float weight, Joueur* joueur);
    ~Input();
    int get_type() const;
    Pos get_pos() const;
    Pos get_offset() const;
    Pos get_pos_graphics() const;
    float get_input() const;
    virtual float get_output();
    virtual float get_weight();
    virtual void Activation();
    void move();
    float check_state();
    virtual void mutation_Input();
    void mutation_position();
    

private:
    Pos m_pos;
    Pos m_pos_graphics;
    Pos m_offset;
    int m_type;
    Joueur* m_joueur;
    float m_output;
    float m_input;
    float m_weight;
    

};