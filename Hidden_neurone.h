#include "GenericNeurone.h"
#include "utils.h"

class Input;
class Joueur;

class Hidden_neurone: public GenericNeurone {
public:
    ~Hidden_neurone();
    Hidden_neurone(int n_Inputs, GenericNeurone** Inputs, Joueur* joueur);
    Hidden_neurone(int n_Inputs, GenericNeurone** Inputs, float* weights, Joueur* joueur);
    int get_n_Inputs() const;
    float* get_weights();
    GenericNeurone* get_Input(int i);
    virtual bool isActive();
    GenericNeurone** get_Inputs();
    void mutation_hidden_neurone();
    void change_weights(int n_inputs);
    void change_input(GenericNeurone** Inputs, int n_inputs);
    void delete_Inputs(int j);
    virtual void Activation();
    virtual float get_output();
    virtual void mutation_Input();
private:
    Joueur* m_joueur;
    GenericNeurone** m_Inputs;
    int m_n_Inputs;
    float m_input;
    double m_output;
    float m_weight;
    float* m_weights;
    float m_Active;
};