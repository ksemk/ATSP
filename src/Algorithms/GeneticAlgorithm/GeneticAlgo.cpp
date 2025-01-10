#include "../../../include/Algorithms/GeneticAlgorithm/GeneticAlgo.h"
#include "GeneticAlgo.h"

GeneticAlgorithm::GeneticAlgorithm(const Matrix &matrix, std::string geneticConfigFilePath)
:matrix(matrix), geneticConfigFilePath(geneticConfigFilePath) {
    // Read the config file
    std::ifstream config_file("../../../" << geneticConfigFilePath);
    if (!config_file.is_open()) {
        std::cerr << "Error: Could not open config file." << std::endl;
        return 1;
    }
    // Parse the config file as JSON
    
    nlohmann::json config_json;
    config_file >> config_json;

    parseParametersFromJSON(config_json);


    // Initial population generation
    population = new int*[populationSize];
    int chromosomeLength = problemSize - 1; // not use 0th city
    for (int i = 0; i < populationSize; ++i) {
        population[i] = new int[chromosomeLength];
        generatePopulation(populationSize);
    }


    offspring = new int*[populationSize]
    for (int = 0; i < populationSize; ++i) {
        offspring[i] = new int[chromosomeLength];
    }

        
}

GeneticAlgorithm::~GeneticAlgorithm()
{
    for (int i = 0; i < populationSize; ++i) {
        delete[] population[i];
    }
    delete[] population;
}

GeneticAlgorithm::runGeneticAlgorithm()
{
}

void const int *GeneticAlgorithm::getBestPath() const
{
    return nullptr;
}

void GeneticAlgorithm::printBestPath() const
{
}

void int GeneticAlgorithm::getBestSolution() const
{
    return void int();
}

const int *GeneticAlgorithm::generateChromosome(int index, int chromosomeLength){
        if (index <= 0 || index > chromosomeLength) {
            std::cerr << "Index out of bounds!" << std::endl;
            return nullptr;
        }
        
        // Fill the chromosome with random cities
        srand(time(nullptr));
        bool visited[size] = {false};
        for (int i = 0; i < chromosomeLength; ++i) {
            int gene;
            do {
                city = rand() % chromosomeLength;
            } while (visited[gene] || gene == 0);
            population[index][i] = gene;
            visited[gene] = true;
        }

        return population[index];
}

void GeneticAlgorithm::generatePopulation(int populationSize){
    for (int i = 0; i < populationSize; i++){
        generateChromosome(i, chromosomeLength);
    }    
}

void GeneticAlgorithm::generateOffspring(int populationSize, int mutationRate, int randomRate){

}

void GeneticAlgorithm::performInversionMutation(int currentChromosome)
{
}

void GeneticAlgorithm::performShuffleMutation(int currentChromosome){
}

void GeneticAlgorithm::performCrossing(){
}

void GeneticAlgorithm::parseParametersFromJSON(const nlohmann::json& config_json) {
    try {
        // Validate and parse the configuration
        const auto& config = config_json.at("geneticAlgorithmConfiguration");

        // Validate and parse crossingSegmentSize
        if (config.contains("crossingSegmentSize") && config["crossingSegmentSize"].is_number_integer()) {
            crossingSegmentSize = config.at("crossingSegmentSize").get<int>();
            if (crossingSegmentSize >= matrix.getSize()) {
                throw std::runtime_error("'crossingSegmentSize' must be less than the size of the matrix.");
            }
        } else {
            throw std::runtime_error("Invalid or missing 'crossingSegmentSize' in configuration.");
        }

        // Validate and parse mutationSegmentSize
        if (config.contains("mutationSegmentSize") && config["mutationSegmentSize"].is_number_integer()) {
            mutationSegmentSize = config.at("mutationSegmentSize").get<int>();
        } else {
            throw std::runtime_error("Invalid or missing 'mutationSegmentSize' in configuration.");
        }

        // Validate and parse mutationType
        if (config.contains("mutationType") && config["mutationType"].is_string()) {
            mutationType = config.at("mutationType").get<std::string>();
            if (mutationType != "inversion" && mutationType != "shuffle") {
                throw std::runtime_error("Invalid 'mutationType' in configuration. Must be 'inversion' or 'shuffle'.");
            }
        } else {
            throw std::runtime_error("Invalid or missing 'mutationType' in configuration.");
        }

        // Validate and parse populationSize
        if (config.contains("populationSize") && config["populationSize"].is_number_integer()) {
            populationSize = config.at("populationSize").get<int>();
            if (populationSize <= 0) {
                throw std::runtime_error("'populationSize' must be greater than 0.");
            }
        } else {
            throw std::runtime_error("Invalid or missing 'populationSize' in configuration.");
        }

        // Validate and parse iterationNum
        if (config.contains("iterationNum") && config["iterationNum"].is_number_integer()) {
            iterationNum = config.at("iterationNum").get<int>();
            if (iterationNum <= 0) {
                throw std::runtime_error("'iterationNum' must be greater than 0.");
            }
        } else {
            throw std::runtime_error("Invalid or missing 'iterationNum' in configuration.");
        }

        // Validate and parse mutationRate
        if (config.contains("mutationRate") && config["mutationRate"].is_number_integer()) {
            mutationRate = config.at("mutationRate").get<int>();
            if (mutationRate < 0 || mutationRate > 100) {
                throw std::runtime_error("'mutationRate' must be between 0 and 100.");
            }
        } else {
            throw std::runtime_error("Invalid or missing 'mutationRate' in configuration.");
        }

        // Validate and parse randomRate
        if (config.contains("randomRate") && config["randomRate"].is_number_integer()) {
            randomRate = config.at("randomRate").get<int>();
            if (randomRate < 0 || randomRate > 100) {
                throw std::runtime_error("'randomRate' must be between 0 and 100.");
            }
        } else {
            throw std::runtime_error("Invalid or missing 'randomRate' in configuration.");
        }
        // Validate that the sum of mutationRate and randomRate is less than 100
        if (mutationRate + randomRate >= 100) {
            throw std::runtime_error("The sum of 'mutationRate' and 'randomRate' must be less than 100.");
        }
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Error parsing genetic algorithm configuration: " << e.what() << std::endl;
        throw;
    } catch (const std::runtime_error& e) {
        std::cerr << "Configuration error: " << e.what() << std::endl;
        throw;
    }
}

int GeneticAlgorithm::calculatePathCost(const int *path){
    return 0;
}