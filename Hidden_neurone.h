
class Input;
class Joueur;
class Hidden_neurone;
#include "GenericNeurone.h"
#include "utils.h"
struct input_type {
    bool is_Input;
    union {
        //Holders
        Hidden_neurone** layer;
        Input** input;
    };
    input_type(Hidden_neurone** value)
    {
        layer = value;
        is_Input = false;
    }
    input_type(Input** value)
    {
        input = value;
        is_Input = true;
    }
    Input** read_as_Input()
    {
        if (is_Input)
        {
            return input;
        }
        else
        {
            return (Input**)layer;
        }
    }
    Hidden_neurone** read_as_layer()
    {
        if (is_Input)
        {
            return layer;
        }
        else
        {
            return (Hidden_neurone**)input;
        }
    }
};



class Hidden_neurone: public GenericNeurone {
public:
    ~Hidden_neurone();
    Hidden_neurone(int n_Inputs, GenericNeurone** Inputs, Joueur* joueur);
    Hidden_neurone(int n_Inputs, GenericNeurone** Inputs, float weight, Joueur* joueur);
    int get_n_Inputs() const;
    virtual float get_weight();
    GenericNeurone* get_Input(int i);
    bool isActive();
    GenericNeurone** get_Inputs();
    void mutation_hidden_neurone();
    void delete_Inputs(int j);
    virtual void Activation();
    virtual float get_output();
    virtual void mutation_Input();
private:
    Joueur* m_joueur;
    GenericNeurone** m_Inputs;
    int m_n_Inputs;
    float m_input;
    float m_output;
    float m_weight;
};