
#include "GenerationPerso.h"

#include <cassert>
static constexpr double EXCES_COEF = 0.5;
static constexpr double POIDSDIFF_COEF = 0.92;
static constexpr double DIFF_LIMITE = 1.0;


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



GenerationHandler::GenerationHandler(const int32_t n_systemes) : m_n_systemes(n_systemes), m_generationId(0), joueur_actuel(0), m_previousBestGen(nullptr)
{
    NetworkSaver NetworkSave;
    isPlaying = true;
    Joueur** newPopulation = init_ptr_array<Joueur>(n_systemes);
    if (!LoadNetworks(&newPopulation)){
        for (int32_t i = 0; i < n_systemes; i++)
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
    std::vector<Specie> Species = sortPopulationBySpecies(populationActu);
    if (bestRewardPop < bestRewardOldPops && bestOldNetwork) {
        printf("Taking an old network as base for the next generation.\n");
        for (size_t i = 0; i < Species.size(); i++) {
            for (size_t j = 0; j < Species[i].networks.size(); j++) {
                Species[i].networks[j]->m_reseau = bestOldNetwork;
            }
        }
    }
    else {//not useful to save a bad generation.
        //mettre la population dans les anciennes
        m_previousBestGen = populationActu;
        if(m_generationId > 1)
            //sauvegarder cette ancienne génération
            SaveNetworks(populationActu);
    }

    //Calcul de la fitness moyenne de chaque espèce et la fitness moyenne de toutes les espèces et fitness total
    size_t nbIndividuTotal = 0;
    size_t AvgRewardGlobal = 0;
    size_t maxReward = 0;
    for (size_t i = 0; i < Species.size(); i++) {
        Species[i].totalReward = 0;
        maxReward = 0;
        for (size_t j = 0; j < Species[i].networks.size(); j++) {
            nbIndividuTotal++;
            Species[i].totalReward += Species[i].networks[j]->m_reward;
            AvgRewardGlobal += Species[i].networks[j]->m_reward;
            if (maxReward < Species[i].networks[j]->m_reward) {
                maxReward = Species[i].networks[j]->m_reward;
            }
        }
        Species[i].maxReward = maxReward;
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
            if (indiceNewSpecie >= m_n_systemes) {
                break;
            }
            NeuralNetwork* newNetwork;
            if (j == 0 && m_generationId != 1) {
                newNetwork = Species[i].networks[0]->m_reseau;
            }
            else {
                newNetwork = new NeuralNetwork(INPUTS_MAX, OUTPUTS_MAX);
                Joueur* firstNet = Species[i].networks[randint(0, size - 1)];
                Joueur* secondNet = Species[i].networks[randint(0, size - 1)];
                newNetwork->CrossOver(firstNet->m_reseau, secondNet->m_reseau, firstNet->m_reward, secondNet->m_reward);
                newNetwork->SpecieParentId = i;
                DoMutation(newNetwork);
            }
            assert(newNetwork != nullptr);
            newPopulation[indiceNewSpecie]->m_reseau = newNetwork;
            newNetwork->sortIncomingConnections();
            newNetwork->topologicalSortNodes();
            newNetwork->DoNodesPairs();
            indiceNewSpecie++;          
        }
        if (indiceNewSpecie >= m_n_systemes) {
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
    int32_t bestOldReward = 0;
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

void  GenerationHandler::update() {
    if (global_ptr->current_lives < 0 && isPlaying)
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

typedef int(__thiscall* FunctionPtr)(void* thisPtr, int32_t arg1);
void GenerationHandler::ResetGame()
{
    printf("Reset game.\n");
    pauseMenu_ptr = *(zPauseMenu**)0x4CF40C;
    char name_to_enter[10] = "";
    sprintf_s(name_to_enter, sizeof(name_to_enter), "Gen%d", m_generationId);
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

int32_t GenerationHandler::getBestReward(Joueur** population, NeuralNetwork** bestNetwork) {
    int32_t best_reward = 0;
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
    size_t nodes_size = network->nodes.size();
    network->nodes.emplace_back(Node{ nodes_size, HIDDEN, 0});
    connections[ConnectId].state = false;
    size_t InnovId = CheckForExistingConnection(connections[ConnectId].Inid, nodes_size);
    connections.emplace_back(Connection{ connections[ConnectId].Inid, nodes_size, connections[ConnectId].weight, true, InnovId});
    InnovId = CheckForExistingConnection(nodes_size, connections[ConnectId].OutId);
    connections.emplace_back(Connection{ nodes_size, connections[ConnectId].OutId, 1, true, InnovId });
    network->connections = connections;
}

void GenerationHandler::AddConnection(size_t in, size_t out, NeuralNetwork* reseau)
{
    size_t InnovId = CheckForExistingConnection(in, out);
    reseau->connections.emplace_back(Connection{ in, out, random_float(), true, InnovId });
}

size_t GenerationHandler::CheckForExistingConnection(size_t in, size_t out) {
    size_t InnovId = 0;
    for (size_t i = 0; i < m_n_systemes; i++) {
        NeuralNetwork* reseau = populationActuelle[i]->m_reseau;
        for (size_t j = 0; j < reseau->connections.size(); j++) {
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
    if (randint(1, 100) < 85 && network->nodesPairs.size() > 0) {
        size_t randomPairIndice = randint(0, network->nodesPairs.size() - 1);
        size_t* pair = network->nodesPairs[randomPairIndice];
        AddConnection(pair[0], pair[1], network);
        network->nodesPairs.erase(network->nodesPairs.begin() + randomPairIndice);
    }
    if (randint(1, 100) < 31 && network->connections.size() > 0) {
        size_t randNb = randint(0, network->connections.size() - 1);
        AddNode(network, randNb);
    }
    for (size_t i = 0; i < network->connections.size(); i++) {
        if (randint(1, 100) < 80)
            network->connections[i].weight += random_bool() ? -0.02 : 0.02;
        else if (randint(1, 100) < 15)
            network->connections[i].weight = random_float();
    }
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
    return (EXCES_COEF * networkDiff) / (max(networkToTest->connections.size() + networkToCompare->connections.size(), 1))
        + POIDSDIFF_COEF * diffPoids;
}
