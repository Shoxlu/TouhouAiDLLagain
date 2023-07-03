
#include "GenerationPerso.h"
#include "Joueur.h"
#include "NeuralNetwork.h"
#include "NetworkSaver.h"
#include "utils.h"
#include <cassert>

#define VK_W 0x57
#define VK_R 0x52
#define VK_D 0x44
#define VK_C 0x43
#define VK_X 0x58
#define MAX_ITERATIONS 100000
constexpr double EXCES_COEF = 0.50;
constexpr double POIDSDIFF_COEF = 0.92;
constexpr double DIFF_LIMITE = 1.00;

extern int32_t* Inputs;
extern zGlobals* global_ptr;
extern zPlayer* player_ptr;
extern int previous_time;
extern BYTE frame_skip;
extern HANDLE hprocess;
extern bool isRendering;
extern NeuralNetwork* preseau;
size_t NbInnov = 0;
extern int INPUTS_MAX;
extern GenerationHandler* generation;
template <class T>
void free_ptr_array(T** ptr, size_t size) {
    for (size_t i = 0; i < size; i++) {
        delete ptr[i];
    }
    delete[] ptr;
}

template <class T>
T** init_ptr_array(size_t size) {
    T** arrayptr = new T * [size];
    for (size_t i = 0; i < size; i++) {
        arrayptr[i] = new T();
    }
    return arrayptr;
}



GenerationHandler::GenerationHandler(const int n_systemes) : m_n_systemes(n_systemes), m_generationId(0), joueur_actuel(0), m_previousBestGen(nullptr)
{
    NetworkSaver NetworkSave;
    isPlaying = true;
    Joueur** newPopulation = init_ptr_array<Joueur>(n_systemes);
    if (!LoadNetworks(&newPopulation)){
        for (int i = 0; i < n_systemes; i++)
        {
            newPopulation[i]->CreateNetwork();
        }
    }
    populationActuelle = newPopulation;
    populationActuelle = newGeneration(newPopulation);
    preseau = populationActuelle[0]->m_reseau;
}

GenerationHandler::~GenerationHandler()
{
    if (populationActuelle) {
        free_ptr_array<Joueur>(populationActuelle, m_n_systemes);
    }
}

bool GenerationHandler::LoadNetworks(Joueur*** pnewPopulation) {
    char filename[512];
    NetworkSaver NetworkSave;
    Joueur** newPopulation = *pnewPopulation;
    for (size_t i = 0; i < m_n_systemes; i++) {
        snprintf(filename, sizeof(filename), "C:/Jeux casse/IA touhou 18/Network%d.csv", i);
        newPopulation[i]->m_reseau = NetworkSave.GetNetwork(filename);
        if (newPopulation[i]->m_reseau == nullptr) {
            return false;
        }
    }
    return true;
}

Joueur** GenerationHandler::newGeneration(Joueur** populationActu)
{
    printf("New generation\n");
    joueur_actuel = 0;
    m_generationId += 1;
    Joueur** newPopulation = init_ptr_array<Joueur>(m_n_systemes);
    //trier la population en espèces
 
    //find max fitness de la population;
    NeuralNetwork* placeHolderNetwork = nullptr;
    size_t bestRewardPop = getBestReward(populationActu, &placeHolderNetwork);
    //s'il y a des anciennes populations, checker si y'a pas un meilleur reseau que le meilleur dans la gen actuelle
    NeuralNetwork* bestOldNetwork = nullptr;
    size_t bestRewardOldPops = findBestNetworkInOldPop(&bestOldNetwork);
    //si c'est le cas : remplacer tous les réseaux de la gen actuelle par le meilleur
    Joueur** populationToSort = nullptr;
    if (bestRewardPop < bestRewardOldPops && bestOldNetwork) {
        printf("Taking an old network as base for the next generation.\n");
        /*for (size_t i = 0; i < Species.size(); i++) {
            for (size_t j = 0; j < Species[i].networks.size(); j++) {
                Species[i].networks[j]->m_reseau = bestOldNetwork;
            }
        }*/
        populationToSort = m_previousBestGen;
    }
    else {//not useful to save a bad generation.
        populationToSort = populationActu;
        m_previousBestGen = populationActu;
        SaveNetworks(populationActu);
    }

    std::vector<Specie> Species = sortPopulationBySpecies(populationToSort);
    //mettre la population dans les anciennes
    
    //sauvegarder cette ancienne génération
    
    //Calcul de la fitness moyenne de chaque espèce et la fitness moyenne de toutes les espèces et fitness total
    size_t nbIndividuTotal = 0;
    size_t AvgRewardGlobal = 0;
    for (size_t i = 0; i < Species.size(); i++) {
        Species[i].totalReward = 0;
        for (size_t j = 0; j < Species[i].networks.size(); j++) {
            nbIndividuTotal++;
            Species[i].totalReward += Species[i].networks[j]->m_reward;
            AvgRewardGlobal += Species[i].networks[j]->m_reward;
        }
        Species[i].avgReward = Species[i].totalReward / Species[i].networks.size();
    }
    assert(nbIndividuTotal == m_n_systemes);
    AvgRewardGlobal = AvgRewardGlobal / nbIndividuTotal;
    //tri des especes pour que les meilleurs placent leurs enfants avant tout
    Species = SortSpeciesByReward(Species);
    //chaque espece va créer un certain nombre d'individu dans la nouvelle population en fonction de si l'espece a un bon fitness ou pas
    size_t NbToCreate = m_n_systemes;
    size_t indiceNewSpecie = 0;
    for (size_t i = 0; i < Species.size(); i++) {
        //- calcul du nombre d'enfants d'une espèce/nb d'individu de l'espèce
        size_t NbSpecieInd = ceil(double(Species[i].networks.size() * double(Species[i].avgReward) / double(AvgRewardGlobal)));
        NbToCreate -= NbSpecieInd;
        if (NbToCreate < 0) {
            NbSpecieInd += NbToCreate;
            NbToCreate = 0;
        }
        Species[i].nbChildren = NbSpecieInd;
        size_t size = Species[i].networks.size();
        //-à partir ce nombre d'individu, faire des crossovers à partir de parents de cet espèce et une mutation et mettre ce crossover dans la nouvelle pop
        for (size_t j = 0; j < NbSpecieInd; j++) {
            if (indiceNewSpecie < m_n_systemes) {
                NeuralNetwork* newNetwork = new NeuralNetwork(INPUTS_MAX, 5);
                
                CrossOver(Species[i].networks[randint(0, size - 1)]->m_reseau, Species[i].networks[randint(0, size - 1)]->m_reseau, newNetwork);
                DoMutation(newNetwork);
                
                newNetwork->SpecieParentId = i;
                newPopulation[indiceNewSpecie]->m_reseau = newNetwork;
                indiceNewSpecie++;
            }
            else {
                break;
            }            
        }
        if (indiceNewSpecie > m_n_systemes) {
            break;
        }
    }
    
    //si l'espèce n'a pas d'enfants, delete l'espèce
    for (size_t i = 0; i < Species.size(); i++) {
        if (Species[i].nbChildren == 0) {
            Species.erase(Species.begin()+i);
        }
    }

    printf("N generation: %d \n", m_generationId);
    return newPopulation;
}

size_t GenerationHandler::findBestNetworkInOldPop(NeuralNetwork** bestOldNetwork) {
    int bestOldReward = 0;
    if (!m_previousBestGen) {
        return 0;
    }
    for (size_t j = 0; j < m_n_systemes; j++) {
        if (m_previousBestGen[j] && m_previousBestGen[j]->m_reward > bestOldReward) {
            // -stocker la meilleure fitness et le meilleur reseau
            bestOldReward = m_previousBestGen[j]->m_reward;
            *bestOldNetwork = m_previousBestGen[j]->m_reseau;
        }
    }
    return bestOldReward;
}


void GenerationHandler::SaveNetworks(Joueur** population) {
    NetworkSaver NetworkSave;
    char name[512];
    for (size_t i = 0; i < m_n_systemes; i++) {
        snprintf(name, 512, "C:/Jeux casse/IA touhou 18/Network%d.csv", i);
        NetworkSave.SaveNetwork(population[i]->m_reseau, name);
    }
}


bool GenerationHandler::joueurMort()
{
    if (global_ptr->current_lives < 0)
        return true;
    return false;
}

void  GenerationHandler::update() {
    if (joueurMort() && isPlaying)
    {   
        joueur_actuel += 1;
        preseau = nullptr;
        isPlaying = false;
        if (joueur_actuel >= m_n_systemes)
        {
            printf("Fin generation\n");
            populationActuelle = newGeneration(populationActuelle);
        }
        HANDLE ResetThread = CreateRemoteThread(hprocess,NULL,NULL,(LPTHREAD_START_ROUTINE)ResetGame_wrap, NULL,NULL,NULL);
    }
    else if (isPlaying) {
        global_ptr->current_lives = 0;
        return populationActuelle[joueur_actuel]->update();
    }
    if (!isPlaying && global_ptr->time_in_stage == 1) {
        printf("New actual player id: %d \n", joueur_actuel);
        isPlaying = true;
        preseau = populationActuelle[joueur_actuel]->m_reseau;
    }
}

void ResetGame_wrap() {
    generation->ResetGame();
}

typedef int(__thiscall* FunctionPtr)(void* thisPtr, int arg1);
void GenerationHandler::ResetGame()
{
    printf("Reset game.\n");
    zPauseMenu* pauseMenu_ptr = *(zPauseMenu**)0x4CF40C;
    char name_to_enter[10] = "";
    sprintf_s(pauseMenu_ptr->name_to_enter, sizeof(pauseMenu_ptr->name_to_enter), "Gen%d", m_generationId);
    FunctionPtr MenuSelect__set_index = reinterpret_cast<FunctionPtr>(0x416BA0);
    while (global_ptr->time_in_stage >= previous_time) {
        if (pauseMenu_ptr->menu_type == 15) {
            memcpy(pauseMenu_ptr->name_to_enter, name_to_enter, sizeof(pauseMenu_ptr->name_to_enter));
            MenuSelect__set_index(&pauseMenu_ptr->char_select_menu, 90);
            *Inputs = Shoot || Enter;
        }
        
        *Inputs |= R; //press(VK_W, 1);
        Sleep(1+16/(frame_skip+1));
    }
    global_ptr->time_in_stage = 0;
    previous_time = 0;
}

std::vector<Specie> GenerationHandler::SortSpeciesByReward(std::vector<Specie> Species) {
    size_t best_id;
    for (size_t i = 0; i < Species.size(); i++) {
        best_id = i;
        for (size_t j = i+1; j < Species.size(); j++) {
            if (Species[j].maxReward > Species[best_id].maxReward) {
                best_id = j;
            }
        }
        Specie temp = Species[i];
        Species[i] = Species[best_id];
        Species[best_id] = temp;
    }
    return Species;
}

int GenerationHandler::getBestReward(Joueur** population, NeuralNetwork** bestNetwork) {
    int best_reward = 0;
    for (size_t i = 0; i < m_n_systemes; i++) {
        if (best_reward < population[i]->m_reward) {
            best_reward = population[i]->m_reward;
            *bestNetwork = population[i]->m_reseau;
        }
    }
    return best_reward;
}

void GenerationHandler::AddNode(NeuralNetwork* network, size_t ConnectId)
{
    std::vector<Connection> connections = network->connections;
    network->nodes.emplace_back(Node{ network->NbNodes, HIDDEN, 0 });
    connections[ConnectId].state = false;
    size_t InnovId = CheckForExistingConnection(connections[ConnectId].Inid, network->NbNodes);
    connections.emplace_back(Connection{ connections[ConnectId].Inid, network->NbNodes, connections[ConnectId].weight, true, InnovId});
    InnovId = CheckForExistingConnection(network->NbNodes, connections[ConnectId].OutId);
    connections.emplace_back(Connection{network->NbNodes, connections[ConnectId].OutId, 1, true, InnovId });
    

    for (size_t i = 0; i < network->nodes.size(); i++) {
        if (i == network->NbNodes || network->nodes[i].type == OUTPUT || network->nodes[network->NbNodes].type == SENSOR)
            continue;
        size_t pair[2] = { network->NbNodes , i};
        network->nodesPairs.emplace_back(pair);
        size_t pair1[2] = {i, network->NbNodes};
        network->nodesPairs.emplace_back(pair1);
    }
    ++network->NbNodes;
    network->NbConnect += 2;
    network->connections = connections;
}

void GenerationHandler::AddConnection(size_t in, size_t out, NeuralNetwork* reseau)
{
    size_t InnovId = CheckForExistingConnection(in, out);
    reseau->connections.emplace_back(Connection{ in, out, random_float(), true, InnovId });
    reseau->NbConnect++;
}

size_t GenerationHandler::CheckForExistingConnection(size_t in, size_t out) {
    size_t InnovId = 0;
    for (size_t i = 0; i < m_n_systemes; i++) {
        NeuralNetwork* reseau = populationActuelle[i]->m_reseau;
        for (size_t j = 0; j < reseau->NbConnect; j++) {
            if (reseau->connections[j].Inid == in && reseau->connections[j].OutId == out) {
                return reseau->connections[j].InnovId;
            }
        }
    }
    ++NbInnov;
    return NbInnov - 1;
}

//just to prevent creating an already existing connection...
bool CheckExistingConnectionInNetwork(size_t in, size_t out, std::vector<Connection> connections) {
    for (size_t i = 0; i < connections.size(); i++)
    {
        if (connections[i].Inid == in && connections[i].OutId == out) {
            return true;
        }
    }
    return false;
}

void GenerationHandler::DoMutation(NeuralNetwork* network) {
    if (randint(0, 100) < 23 && network->nodesPairs.size() > 0) {
        size_t randomPairIndice = randint(0, network->nodesPairs.size() - 1);
        size_t* pair = network->nodesPairs[randomPairIndice];
        AddConnection(pair[0], pair[1], network);
        network->nodesPairs.erase(network->nodesPairs.begin() + randomPairIndice);
    }
    if (randint(0, 100) < 85 && network->connections.size() > 0) {
        size_t randNb = randint(0, network->NbConnect - 1);
        AddNode(network, randNb);
    }
    for (size_t i = 0; i < network->connections.size(); i++) {
        if (randint(0, 100) < 95)
            network->connections[i].weight += random_float() * 0.2;
        else if(randint(0, 100) < 15)
            network->connections[i].weight = random_float();
    }
}
/*Connection* test() {
    return nullptr;
}
Connection* GetConnectionById(std::vector<Connection>* connections, size_t InnovId) {
    for (size_t i = 0; i < connections->size(); i++) {
        if ((*connections)[i].InnovId == InnovId) {
            return &(*connections)[i];
        }
    }
    return nullptr;
}*/

void GenerationHandler::CrossOver(NeuralNetwork* parent1, NeuralNetwork* parent2, NeuralNetwork* crossSpring) {
    /*size_t max_NbInnov = 0;
    if (parent1->connections.size() > 0 && parent2->connections.size() > 0) {
        max_NbInnov = max(parent1->connections.back().InnovId, parent2->connections.back().InnovId);
    }
    else if (parent2->connections.size() > 0)
        max_NbInnov = parent2->connections.back().InnovId;
    else if (parent1->connections.size() > 0)
        max_NbInnov = parent1->connections.size();
    Connection* connection = nullptr;
    Connection* connection1 = nullptr;
    for (size_t InnovId = 0; InnovId < max_NbInnov + 1; InnovId++) {
        connection = GetConnectionById(&parent1->connections, InnovId);
        connection1 = GetConnectionById(&parent2->connections, InnovId);
        if (connection && connection1) {
            //we'll assume that the crossSpring is getting parent1 connection weight in this case
            connection->state &= connection->state;
            newConnections.emplace_back(connection);
        }
        else if (connection) {
            newConnections.emplace_back(connection);
        }
        else if (connection1) {
            newConnections.emplace_back(connection1);
        }
    }*/
    std::vector<Connection> newConnections;
    newConnections = parent2->connections;
    bool found = false;
    for (size_t i = 0; i < parent1->NbConnect; i++) {
        for (size_t j = 0; j < parent2->NbConnect; j++) {
            if (parent1->connections[i].InnovId == parent2->connections[j].InnovId) {
                newConnections[j].state &= parent1->connections[i].state;
                found = true;
                break;
            }
        }
        if(!found)
            newConnections.emplace_back(parent1->connections[i]);
        found = false;
    }
    crossSpring->connections = newConnections;
    crossSpring->NbConnect = newConnections.size();
    if (parent1->NbNodes > parent2->NbNodes)
        crossSpring->nodes = parent1->nodes;
    else
        crossSpring->nodes = parent2->nodes;
    crossSpring->NbNodes = crossSpring->nodes.size();
    std::vector<size_t*> nodesPairs;
    for (size_t j = 0; j < crossSpring->nodes.size(); j++) {
        for (size_t i = 0; i < crossSpring->nodes.size(); i++) {
            if (i == j || crossSpring->nodes[i].type == OUTPUT || crossSpring->nodes[j].type == SENSOR)
                continue;
            size_t* pair = new size_t[2];
            pair[0] = i;
            pair[1] = j;
            nodesPairs.emplace_back(pair);
        }
    }
    for (size_t i = 0; i < crossSpring->nodesPairs.size(); i++) {
        delete[] crossSpring->nodesPairs[i];
    }
    crossSpring->nodesPairs = nodesPairs;
}


//at this point i copied someone else code (changed it a bit to correspond to my architecture)
std::vector<Specie> GenerationHandler::sortPopulationBySpecies(Joueur** population)
{
    std::vector<Specie> Species;
    Species.emplace_back(Specie());
    Species[0].networks.emplace_back(population[m_n_systemes-1]);
    for (size_t i = 0; i < m_n_systemes - 1; i++) {
        bool found = false;
        for (size_t j = 0; j < Species.size(); j++) {
            size_t indice = randint(0, Species[j].networks.size()-1);
            NeuralNetwork* networkToCompare = Species[j].networks[indice]->m_reseau;
            double score = getScore(population[i]->m_reseau, networkToCompare);
            if (score < DIFF_LIMITE) {
                Species[j].networks.emplace_back(population[i]);
                found = true;
                break;
            }
        }
        if (found == false) {
            Species.emplace_back(Specie());
            Species[Species.size() - 1].networks.emplace_back(population[i]);
        }
    }
    return Species;
}

size_t GenerationHandler::getNetworkDiff(NeuralNetwork* network1, NeuralNetwork* network2, double* diffPoids)
{
    size_t nbSameConnect = 0;
    size_t size1 = network1->connections.size();
    size_t size2 = network2->connections.size();
    double total = 0;
    for (size_t i = 0; i < size1; i++) {
        for (size_t j = 0; j < size2; j++) {
            if (network1->connections[i].InnovId == network2->connections[j].InnovId) {
                ++nbSameConnect;
                total += abs(network1->connections[i].weight - network2->connections[j].weight);
            }
        }
    }
    if (diffPoids) {
        if (nbSameConnect == 0)
            *diffPoids = 100000;
        else
            *diffPoids = total / nbSameConnect;
    }
    return size1 + size2 - 2 * nbSameConnect;
}

double GenerationHandler::getScore(NeuralNetwork* networkToTest, NeuralNetwork* networkToCompare)
{
    //thanks Laupok
    double diffPoids = 0;
    double networkDiff = getNetworkDiff(networkToTest, networkToCompare, &diffPoids);
    return (EXCES_COEF * networkDiff) / (max(networkToTest->NbConnect + networkToCompare->NbConnect, 1))
        + POIDSDIFF_COEF * diffPoids;
}
