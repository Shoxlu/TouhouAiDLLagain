
#include "utils.h"
#include "GenericNeurone.h"
class Hidden_neurone;
class Joueur;
class Input;
struct input_type;
class Window;

class Reseau {

public:
    ~Reseau();
    
    Reseau(int n_layers, int n_inputs_all, int n_hidden_neurones, Joueur* joueur);
    Reseau(int n_layers, int n_inputs_all, int n_hidden_neurones, GenericNeurone*** layers, Input** inputs, Joueur* joueur);
    
    void Create_hidden_layer(int n_inputs, GenericNeurone** inputs, int n, int n_ouputs);
    void Create_Inputs();
    void Reset_m_layers();
    void input_into_layer();
    
    int get_n_Hidden_layer() const;
    int get_n_Hidden_neurones() const;
    Joueur* get_joueur();
    int get_n_inputs();
    GenericNeurone* get_Hidden_layer(int i);
    GenericNeurone*** get_Hidden_layers();
    Input** get_inputs();
    Dir* get_moves();
    void update();

    
    
    void mutation_reseau();
    void add_input();
    void add_hidden_neurone();
    void add_hidden_layer();
    void remove_input(int j);
    void delete_Hidden_layer(int j);

private:
    Joueur* m_joueur;
    GenericNeurone** m_layer;
    GenericNeurone*** m_layers;
    int m_n_layers;
    int m_n_hidden_neurones;
    int m_n_inputs;
    Input** m_inputs;
    int m_n_outputs;
    Dir moves[6];
    float* weights;
};