#include "GenericNeurone.h"
#include "utils.h"

class Input;
class Joueur;

class Hidden_neurone: public GenericNeurone {
public:
    ~Hidden_neurone();
    Hidden_neurone(int n_Inputs, GenericNeurone** Inputs,int n_outputs, float* weights, Joueur* joueur);
    int get_n_Inputs() const;
    float* get_weights();
    GenericNeurone* get_Input(int i);
    virtual bool isActive();
    GenericNeurone** get_Inputs();
    void mutation_hidden_neurone();
    void change_weights(int n_ouputs);
    void change_input(GenericNeurone** Inputs, int n_inputs);
    virtual float get_output(int k, int self_id);
    virtual void mutation_Input();
    virtual int get_n_outputs();
private:
    Joueur* m_joueur;
    GenericNeurone** m_inputs;
    int m_n_inputs;
    int m_n_outputs;
    float m_input;
    double m_output;
    float m_weight;
    float* m_weights;
    float m_isActive;
};