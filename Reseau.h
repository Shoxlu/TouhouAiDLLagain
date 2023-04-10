
#include "List.h"
#include "utils.h"
#include "GenericNeurone.h"
class Hidden_neurone;
class Joueur;
class Input;
struct input_type;
class Reseau {

public:
    ~Reseau();
    void input_into_layer();
    Reseau(int n_layers, int n_inputs_all, int n_hidden_neurones, Joueur* joueur);
    void Reset_m_layers();
    Reseau(int n_layers, int n_inputs_all, int n_hidden_neurones, GenericNeurone*** layers, Input** inputs, GenericNeurone** output, Joueur* joueur);
    void Create_hidden_layer(int n_inputs, GenericNeurone** inputs, int n);
    int get_n_Hidden_layer() const;
    int get_n_Hidden_neurones() const;
    GenericNeurone* get_Hidden_layer(int i);
    GenericNeurone*** get_Hidden_layers();
    GenericNeurone** get_outputs();
    Input** get_inputs();
    void update();
    int get_n_inputs();
    Joueur* get_joueur();
    void Create_Inputs();
    void mutation_inputs();
    void Reset_m_inputs();
    void mutation_reseau();
    void delete_Inputs(int j);
    void mutation_Hidden_layers();
    void mutation_reseau_add_Hidden_layer();
    void delete_Hidden_layer(int j);
private:
    Joueur* m_joueur;
    GenericNeurone** m_layer;
    GenericNeurone** m_outputs;
    int m_n_layers;
    GenericNeurone*** m_layers;
    int m_n_hidden_neurones;
    int m_n_inputs;
    Input** m_inputs;
    int m_n_outputs;
    Dir moves[6];
    float* weights;
};