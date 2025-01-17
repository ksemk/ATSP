#include "../../include/Algorithms/GeneticAlgo.h"

// Constructor
GeneticAlgorithm::GeneticAlgorithm(const Matrix& matrix, std::string geneticConfigFilePath)
    : matrix(matrix),                     // Initialize the reference
      problemSize(matrix.getSize()),     // Assuming Matrix has a getSize() method
      population(nullptr),               // Initialize pointers to nullptr
      offspring(nullptr),
      offspringPopulation(nullptr),
      bestPath(nullptr),
      bestSolution(std::numeric_limits<int>::max()), // Set a large default value for bestSolution
      geneticConfigFilePath(std::move(geneticConfigFilePath)), // Use std::move for efficiency
      populationSize(0),                 // Initialize with a sensible default
      iterationNum(0),
      crossingSegmentSizeRate(0),
      mutationSegmentSizeRate(0),
      mutationType('i'),                 // Default to 'i' for inversion
      mutationRate(0),
      randomRate(0),
      randomRateNewGen(0) {
    // Construct the full path to the config file
    std::string fullPath = this->geneticConfigFilePath;

    //Debugging
    // std::cout << "Full path to config file: " << fullPath << std::endl;

    // Read the config file
    std::ifstream config_file(fullPath);
    if (!config_file.is_open()) {
        throw std::runtime_error("Could not open config file: " + fullPath);
    }

    // Parse the config file as JSON
    nlohmann::json config_json;
    config_file >> config_json;
    parseParametersFromJSON(config_json);

    // Initial population generation
    population = new int*[populationSize];
    offspringPopulation = new int*[populationSize];
    int chromosomeLength = matrix.getSize() - 1; // Exclude the 0th city for ATSP

    try {
        for (int i = 0; i < populationSize; ++i) {
            population[i] = generateChromosome(chromosomeLength);
            offspringPopulation[i] = new int[chromosomeLength];
        }
    } catch (...) {
        // Clean up allocated memory in case of an exception
        for (int i = 0; i < populationSize; ++i) {
            delete[] population[i];
            delete[] offspringPopulation[i];
        }
        delete[] population;
        delete[] offspringPopulation;
        throw; // Re-throw the exception
    }
}


// Destructor
GeneticAlgorithm::~GeneticAlgorithm() {
    // std::cout << "Destroying GeneticAlgorithm object" << std::endl;
    for (int i = 0; i < populationSize; ++i) {
        delete[] population[i];
        delete[] offspringPopulation[i];
    }
    delete[] population;
    delete[] offspringPopulation;
}

// // Generate initial population
// void GeneticAlgorithm::generatePopulation(int populationSize, int chromosomeLength) {
//     for (int i = 0; i < populationSize; ++i) {
//         std::cout<<"Chromosome nr: " << i << std::endl;
//         population[i] = generateChromosome(chromosomeLength);
//     }
// }

// Generate a single chromosome
int* GeneticAlgorithm::generateChromosome(int chromosomeLength) {
    if (chromosomeLength <= 0) {
        throw std::invalid_argument("Chromosome length must be positive.");
    }

    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }

    // Create a 1D array to store city IDs
    int* chromosome = new int[chromosomeLength];
    for (int i = 0; i < chromosomeLength; ++i) {
        chromosome[i] = i + 1; // Cities start from 1
    }

    // Shuffle the chromosome
    for (int i = chromosomeLength - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(chromosome[i], chromosome[j]);
    }

    // debugging
    // for (int i = 0; i < chromosomeLength; ++i) {
    //     std::cout << chromosome[i] << " ";
    // }
    // std::cout << std::endl;


    return chromosome;
}


int diversityThreshold = 0; // At least 30% difference

// Generate offspring with mutation and crossing
void GeneticAlgorithm::generateOffspring(int chromosomeLength) {
    int offspringIndex = 0;

    // Generate random chromosomes
    for (; offspringIndex < populationSize * (randomRate / 100.0); ++offspringIndex) {
        offspringPopulation[offspringIndex] = generateChromosome(chromosomeLength);
    }

    // Perform mutation
    for (; offspringIndex < populationSize * ((randomRate + mutationRate) / 100.0); ++offspringIndex) {
        int parentIndex = rand() % populationSize;
        offspringPopulation[offspringIndex] = new int[chromosomeLength];

        // Copy parent chromosome
        for (int i = 0; i < chromosomeLength; ++i) {
            offspringPopulation[offspringIndex][i] = population[parentIndex][i];
        }

        // Apply mutation
        if (mutationType == 'i') {
            performInversionMutation(offspringIndex, chromosomeLength, mutationSegmentSizeRate);
        } else if (mutationType == 's') {
            performShuffleMutation(offspringIndex, chromosomeLength, mutationSegmentSizeRate);
        }
    }

    // Perform crossover with diversification mechanism
    for (; offspringIndex < populationSize; offspringIndex += 2) {
        int parent1 = rand() % populationSize;
        int parent2;
        int retries = 0;
        const int maxRetries = 10;

        // Ensure parent2 is diverse from parent1
        do {
            parent2 = rand() % populationSize;
            retries++;
        } while (calculateDiversity(population[parent1], population[parent2], chromosomeLength) < diversityThreshold && retries < maxRetries);

        // Diversification mechanism if retries exceed limit
        if (retries >= maxRetries) {
            if (rand() % 2 == 0) {
                // Randomly generate a new chromosome as parent2
                parent2 = offspringIndex; // Use a new random chromosome
                offspringPopulation[parent2] = generateChromosome(chromosomeLength);
            } else {
                // Apply mutation to an existing chromosome to create diversity
                parent2 = rand() % populationSize;
                offspringPopulation[parent2] = new int[chromosomeLength];
                for (int i = 0; i < chromosomeLength; ++i) {
                    offspringPopulation[parent2][i] = population[parent2][i];
                }
                performInversionMutation(parent2, chromosomeLength, mutationSegmentSizeRate);
            }
        }

        performCrossing(parent1, parent2, offspringIndex, chromosomeLength, crossingSegmentSizeRate);
    }
}


int GeneticAlgorithm::calculateDiversity(int* chromosome1, int* chromosome2, int length) {
    int diversity = 0;
    for (int i = 0; i < length; ++i) {
        if (chromosome1[i] != chromosome2[i]) {
            diversity++;
        }
    }
    return diversity;
}



// Perform inversion mutation
void GeneticAlgorithm::performInversionMutation(int currentChromosome, int chromosomeLength, int mutationSegmentSizeRate) {
    // Calculate the actual segment size based on the mutationSegmentSizeRate rate (1 to 100)
    int segmentSize = (mutationSegmentSizeRate * chromosomeLength) / 100;

    // Ensure the segment size is at least 1
    if (segmentSize < 1) {
        segmentSize = 1;
    }

    // Choose the start and end indices of the inversion segment
    int start = rand() % (chromosomeLength - segmentSize + 1);
    int end = start + segmentSize - 1;

    // Perform the inversion
    while (start < end) {
        int temp = offspringPopulation[currentChromosome][start];
        offspringPopulation[currentChromosome][start] = offspringPopulation[currentChromosome][end];
        offspringPopulation[currentChromosome][end] = temp;
        start++;
        end--;
    }
}


// Perform shuffle mutation
void GeneticAlgorithm::performShuffleMutation(int currentChromosome, int chromosomeLength, int mutationSegmentSizeRate) {
    // Calculate the actual segment size based on the mutationSegmentSizeRate rate (1 to 100)
    int segmentSize = (mutationSegmentSizeRate * chromosomeLength) / 100;

    // Ensure the segment size is at least 1
    if (segmentSize < 1) {
        segmentSize = 1;
    }

    // Choose the start index for the shuffle
    int start = rand() % (chromosomeLength - segmentSize + 1);

    // Perform Fisher-Yates shuffle within the segment
    for (int i = 0; i < segmentSize - 1; ++i) {
        int randomIndex = start + (rand() % (segmentSize - i));
        int currentIndex = start + i;

        // Swap the elements
        int temp = offspringPopulation[currentChromosome][currentIndex];
        offspringPopulation[currentChromosome][currentIndex] = offspringPopulation[currentChromosome][randomIndex];
        offspringPopulation[currentChromosome][randomIndex] = temp;
    }
}


// Perform crossing
void GeneticAlgorithm::performCrossing(int parent1, int parent2, int offspringIndex, int chromosomeLength, int crossingSegmentSizeRate) {
    int retries = 0;
    const int maxRetries = 10; // Maximum retries to ensure diversity
    const double diversityThreshold = chromosomeLength * 0.3; // At least 30% diversity

    // Ensure parents are diverse enough
    while (calculateDiversity(population[parent1], population[parent2], chromosomeLength) < diversityThreshold && retries < maxRetries) {
        parent2 = rand() % populationSize; // Select a new random parent2
        retries++;
    }

    // Diversification mechanism: If retries exceed limit
    if (retries >= maxRetries) {
        if (rand() % 2 == 0) {
            // Option 1: Replace parent2 with a new random chromosome
            parent2 = offspringIndex;
            offspringPopulation[parent2] = generateChromosome(chromosomeLength);
        } else {
            // Option 2: Apply mutation to ensure diversity
            parent2 = rand() % populationSize;
            int* mutatedChromosome = new int[chromosomeLength];
            for (int i = 0; i < chromosomeLength; ++i) {
                mutatedChromosome[i] = population[parent2][i];
            }
            performInversionMutation(parent2, chromosomeLength, mutationSegmentSizeRate);
            population[parent2] = mutatedChromosome;
        }
    }

    // Allocate memory for two offspring
    offspringPopulation[offspringIndex] = new int[chromosomeLength];
    offspringPopulation[offspringIndex + 1] = new int[chromosomeLength];

    // Initialize used markers for crossover
    bool* used1 = new bool[chromosomeLength + 1](); // For offspring 1
    bool* used2 = new bool[chromosomeLength + 1](); // For offspring 2

    // Calculate the actual segment size based on the crossingSegmentSizeRate rate (1 to 100)
    int segmentSize = (crossingSegmentSizeRate * chromosomeLength) / 100;

    // Ensure the segment size is at least 1
    if (segmentSize < 1) {
        segmentSize = 1;
    }

    // Choose random segment for crossover
    int start = rand() % (chromosomeLength - segmentSize + 1);
    int end = start + segmentSize;

    // Copy the segment from parents to offspring
    for (int i = start; i < end; ++i) {
        offspringPopulation[offspringIndex][i] = population[parent1][i];
        offspringPopulation[offspringIndex + 1][i] = population[parent2][i];

        used1[population[parent1][i]] = true;
        used2[population[parent2][i]] = true;
    }

    // Fill remaining genes
    int index1 = 0, index2 = 0;
    for (int i = 0; i < chromosomeLength; ++i) {
        // Fill offspring 1
        if (i < start || i >= end) {
            while (used1[population[parent2][index1]]) {
                index1++;
            }
            offspringPopulation[offspringIndex][i] = population[parent2][index1];
            used1[population[parent2][index1]] = true;
            index1++;
        }

        // Fill offspring 2
        if (i < start || i >= end) {
            while (used2[population[parent1][index2]]) {
                index2++;
            }
            offspringPopulation[offspringIndex + 1][i] = population[parent1][index2];
            used2[population[parent1][index2]] = true;
            index2++;
        }
    }

    // Clean up
    delete[] used1;
    delete[] used2;
}

void swap(int*& a, int*& b) {
    int* temp = a;
    a = b;
    b = temp;
}

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void GeneticAlgorithm::selection(int chromosomeLength) {
    // Combine population and offspring
    int totalSize = 2 * populationSize;
    int** combinedPopulation = new int*[totalSize];
    int* fitnessScores = new int[totalSize];

    for (int i = 0; i < populationSize; ++i) {
        combinedPopulation[i] = population[i];
        fitnessScores[i] = calculatePathCost(population[i]);
    }
    for (int i = 0; i < populationSize; ++i) {
        combinedPopulation[populationSize + i] = offspringPopulation[i];
        fitnessScores[populationSize + i] = calculatePathCost(offspringPopulation[i]);
    }

    // Sort combinedPopulation by fitnessScores
    for (int i = 0; i < totalSize - 1; ++i) {
        for (int j = i + 1; j < totalSize; ++j) {
            if (fitnessScores[i] > fitnessScores[j]) {
                swap(fitnessScores[i], fitnessScores[j]); // Use custom swap for integers
                swap(combinedPopulation[i], combinedPopulation[j]); // Use custom swap for pointers
            }
        }
    }

    // Calculate the number of chromosomes to replace for diversity
    int numDiverseChromosomes = (populationSize * randomRateNewGen) / 100;

    // Select top chromosomes for the next generation
    for (int i = 0; i < populationSize - numDiverseChromosomes; ++i) {
        for (int j = 0; j < chromosomeLength; ++j) {
            population[i][j] = combinedPopulation[i][j];
        }
    }

    // Introduce diversity by picking from the lower half of the sorted population
    int startIndex = populationSize; // Start picking from the offspring
    for (int i = populationSize - numDiverseChromosomes; i < populationSize; ++i) {
        int randomIndex = startIndex + (rand() % (totalSize - startIndex)); // Pick from the lower-ranked half
        delete[] population[i]; // Clean up old chromosome
        population[i] = new int[chromosomeLength];

        // Copy the randomly picked chromosome
        for (int j = 0; j < chromosomeLength; ++j) {
            population[i][j] = combinedPopulation[randomIndex][j];
        }
    }

    // Clean up
    delete[] fitnessScores;
    delete[] combinedPopulation;
}



void GeneticAlgorithm::parseParametersFromJSON(const nlohmann::json& config_json) {
    try {
        // Validate and parse the configuration
        const auto& config = config_json.at("geneticAlgorithmConfiguration");

        // Validate and parse crossingSegmentSizeRate
        if (config.contains("crossingSegmentSizeRate") && config["crossingSegmentSizeRate"].is_number_integer()) {
            crossingSegmentSizeRate = config.at("crossingSegmentSizeRate").get<int>();
            // if (crossingSegmentSizeRate >= matrix.getSize()) {
            //     throw std::runtime_error("'crossingSegmentSizeRate' must be less than the size of the matrix.");
            // }
        } else {
            throw std::runtime_error("Invalid or missing 'crossingSegmentSizeRate' in configuration.");
        }

        // Validate and parse mutationSegmentSizeRate
        if (config.contains("mutationSegmentSizeRate") && config["mutationSegmentSizeRate"].is_number_integer()) {
            mutationSegmentSizeRate = config.at("mutationSegmentSizeRate").get<int>();
        } else {
            throw std::runtime_error("Invalid or missing 'mutationSegmentSizeRate' in configuration.");
        }

        // Validate and parse mutationType (as a char)
        if (config.contains("mutationType") && config["mutationType"].is_string()) {
            std::string mutationTypeStr = config.at("mutationType").get<std::string>();
            if (mutationTypeStr.length() != 1) {
                throw std::runtime_error("Invalid 'mutationType'. It must be a single character ('i' or 's').");
            }
            mutationType = mutationTypeStr[0]; // Convert the first character of the string to a char
            if (mutationType != 'i' && mutationType != 's') {
                throw std::runtime_error("Invalid 'mutationType' in configuration. Must be 'i' (inversion) or 's' (shuffle).");
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

        // Validate and parse randomRateNewGen
        if (config.contains("randomRateNewGen") && config["randomRateNewGen"].is_number_integer()) {
            randomRateNewGen = config.at("randomRateNewGen").get<int>();
            if (randomRateNewGen < 0 || randomRateNewGen > 100) {
            throw std::runtime_error("'randomRateNewGen' must be between 0 and 100.");
            }
        } else {
            throw std::runtime_error("Invalid or missing 'randomRateNewGen' in configuration.");
        }
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Error parsing genetic algorithm configuration: " << e.what() << std::endl;
        throw;
    } catch (const std::runtime_error& e) {
        std::cerr << "Configuration error: " << e.what() << std::endl;
        throw;
    }
}



int GeneticAlgorithm::calculatePathCost(const int *path) {
    int totalCost = 0;

    // Include city 0 as the start and end point
    int prevCity = 0; // Starting from city 0

    for (int i = 0; i < problemSize - 1; ++i) {
        int currentCity = path[i];
        totalCost += matrix.getCost(prevCity, currentCity); // Add cost from prevCity to currentCity
        prevCity = currentCity;
    }

    // Add cost from the last city back to city 0
    totalCost += matrix.getCost(prevCity, 0);

    return totalCost;
}


void GeneticAlgorithm::runGeneticAlgorithm() {
    // Initialize variables for tracking the best solution
    int bestCost = INT_MAX;
    int* bestPath = nullptr;

    for (int iteration = 0; iteration < iterationNum; ++iteration) {
        // Step 1: Generate offspring
        generateOffspring(problemSize - 1);

        // Step 2: Combine population and offspring and select the best chromosomes
        selection(problemSize - 1);

        // Track the best solution
        int currentBestCost = calculatePathCost(population[0]);
        if (currentBestCost < bestCost) {
            bestCost = currentBestCost;
            if (bestPath) {
                delete[] bestPath;
            }
            bestPath = new int[problemSize - 1];
            for (int i = 0; i < problemSize - 1; ++i) {
                bestPath[i] = population[0][i];
            }
        }

        // std::cout << "Iteration " << iteration + 1 << ": Best cost = " << bestCost << std::endl;
    }

    // // Output the best solution
    // std::cout << "Best solution found:" << std::endl;
    // std::cout << "Path: 0 ";
    // for (int i = 0; i < problemSize - 1; ++i) {
    //     std::cout << bestPath[i] << " ";
    // }
    // std::cout << "0" << std::endl;
    // std::cout << "Cost: " << bestCost << std::endl;

    // Free memory for best path
    if (bestPath) {
        delete[] bestPath;
    }
}


int* GeneticAlgorithm::getBestPath() {
    // Allocate memory for the best path
    int* bestPath = new int[problemSize + 1];

    // Ensure problem size is sufficient to form a valid path
    if (problemSize < 3) {
        std::cerr << "Error: problemSize must be at least 3 to form a valid path." << std::endl;
        delete[] bestPath; // Avoid memory leak
        return nullptr;    // Return null to indicate failure
    }

    // Add the starting city (0) to the beginning and end of the path
    bestPath[0] = 0; // Starting city
    for (int i = 1; i <= problemSize - 1; ++i) {
        bestPath[i] = population[0][i - 1]; // Fill from the best chromosome in the population
    }
    bestPath[problemSize] = 0; // Return to the starting city


    return bestPath; // Return the dynamically allocated best path
}


int GeneticAlgorithm::getBestSolution() {
    // Calculate the cost of the best chromosome in the population
    return calculatePathCost(population[0]);
}

float GeneticAlgorithm::getMutationRate()
{
    return mutationRate;
}

float GeneticAlgorithm::getCrossOverRate()
{
    
    return 100 - mutationRate - randomRate;
}

int GeneticAlgorithm::getPopulationSize()
{
    return populationSize;
}
int GeneticAlgorithm::getRandomRate() {
    return randomRate;
}

int GeneticAlgorithm::getCrossingSegmentSizeRate() {
    return crossingSegmentSizeRate;
}

int GeneticAlgorithm::getMutationSegmentSizeRate() {
    return mutationSegmentSizeRate;
}

char GeneticAlgorithm::getMutationType() {
    return mutationType;
}

int GeneticAlgorithm::getIterationNum() {
    return iterationNum;
}

int GeneticAlgorithm::getRandomRateNewGen() {
    return randomRateNewGen;
}

void GeneticAlgorithm::printSolution() {
    int* bestPath = getBestPath();
    if (bestPath == nullptr) {
        std::cerr << "Error: Unable to retrieve the best path." << std::endl;
        return;
    }

    int bestCost = getBestSolution();

    std::cout << "Best solution found:" << std::endl;
    std::cout << "Path: ";
    for (int i = 0; i <= problemSize; ++i) { // Iterate over problemSize + 1 to include the return to 0
        std::cout << bestPath[i] << " ";
    }
    std::cout << std::endl;

    if (bestCost < 0) { // Optional: Check for invalid cost values
        std::cerr << "Error: Invalid cost value retrieved." << std::endl;
    } else {
        std::cout << "Cost: " << bestCost << std::endl;
    }

    delete[] bestPath; // Free the dynamically allocated memory
}


