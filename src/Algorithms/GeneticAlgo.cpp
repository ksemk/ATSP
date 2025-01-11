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
      crossingSegmentSize(0),
      mutationSegmentSize(0),
      mutationType('i'),                 // Default to 'i' for inversion
      mutationRate(0),
      randomRate(0) {
    // Construct the full path to the config file
    std::string fullPath = this->geneticConfigFilePath;

    //Debugging
    std::cout << "Full path to config file: " << fullPath << std::endl;

    // Read the config file
    std::ifstream config_file(fullPath);
    if (!config_file.is_open()) {
    std::cerr << "Error opening file: " << strerror(errno) << std::endl;
    throw std::runtime_error("Could not open config file for ga");
    }

    // Parse the config file as JSON
    nlohmann::json config_json;
    config_file >> config_json;
    parseParametersFromJSON(config_json);

    // Initial population generation
    population = new int*[populationSize];
    offspringPopulation = new int*[populationSize];
    int chromosomeLength = matrix.getSize() - 1; // Exclude the 0th city for TSP

    for (int i = 0; i < populationSize; ++i) {
        population[i] = generateChromosome(chromosomeLength);
        offspringPopulation[i] = new int[chromosomeLength];
    }
}

// Destructor
GeneticAlgorithm::~GeneticAlgorithm() {
    for (int i = 0; i < populationSize; ++i) {
        delete[] population[i];
        delete[] offspringPopulation[i];
    }
    delete[] population;
    delete[] offspringPopulation;
}

// Generate initial population
void GeneticAlgorithm::generatePopulation(int populationSize, int chromosomeLength) {
    for (int i = 0; i < populationSize; ++i) {
        population[i] = generateChromosome(chromosomeLength);
    }
}

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

    return chromosome;
}

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
            performInversionMutation(offspringIndex, chromosomeLength, mutationSegmentSize);
        } else if (mutationType == 's') {
            performShuffleMutation(offspringIndex, chromosomeLength, mutationSegmentSize);
        }
    }

    // Perform crossover
    for (; offspringIndex < populationSize; offspringIndex += 2) {
        int parent1 = rand() % populationSize;
        int parent2 = rand() % populationSize;

        performCrossing(parent1, parent2, offspringIndex, chromosomeLength, crossingSegmentSize);
    }
}


// Perform inversion mutation
void GeneticAlgorithm::performInversionMutation(int currentChromosome, int chromosomeLength, int mutationSegmentSize) {
    // Choose the start and end indices of the inversion segment
    int start = rand() % (chromosomeLength - mutationSegmentSize + 1);
    int end = start + mutationSegmentSize - 1;

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
void GeneticAlgorithm::performShuffleMutation(int currentChromosome, int chromosomeLength, int mutationSegmentSize) {
    // Choose the start index for the shuffle
    int start = rand() % (chromosomeLength - mutationSegmentSize + 1);

    // Perform Fisher-Yates shuffle within the segment
    for (int i = 0; i < mutationSegmentSize - 1; ++i) {
        int randomIndex = start + (rand() % (mutationSegmentSize - i));
        int currentIndex = start + i;

        // Swap the elements
        int temp = offspringPopulation[currentChromosome][currentIndex];
        offspringPopulation[currentChromosome][currentIndex] = offspringPopulation[currentChromosome][randomIndex];
        offspringPopulation[currentChromosome][randomIndex] = temp;
    }
}


// Perform crossing
void GeneticAlgorithm::performCrossing(int parent1, int parent2, int offspringIndex, int chromosomeLength, int crossingSegmentSize) {
    // Allocate memory for two offspring
    offspringPopulation[offspringIndex] = new int[chromosomeLength];
    offspringPopulation[offspringIndex + 1] = new int[chromosomeLength];

    // Initialize used markers for crossover
    bool* used1 = new bool[chromosomeLength + 1](); // For offspring 1
    bool* used2 = new bool[chromosomeLength + 1](); // For offspring 2

    // Choose random segment for crossover
    int start = rand() % (chromosomeLength - crossingSegmentSize + 1);
    int end = start + crossingSegmentSize;

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
                std::swap(fitnessScores[i], fitnessScores[j]);
                std::swap(combinedPopulation[i], combinedPopulation[j]);
            }
        }
    }

    // Select top chromosomes for the next generation
    for (int i = 0; i < populationSize; ++i) {
        for (int j = 0; j < chromosomeLength; ++j) {
            population[i][j] = combinedPopulation[i][j];
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
        for (int i = 0; i < populationSize; ++i) {
            if (i < populationSize * randomRate / 100) {
                // Random chromosome generation
                for (int j = 0; j < problemSize - 1; ++j) {
                    offspringPopulation[i][j] = rand() % (problemSize - 1) + 1;
                }
            } else if (i < populationSize * (randomRate + mutationRate) / 100) {
                // Perform mutation
                if (mutationType == 'i') {
                    performInversionMutation(i, problemSize - 1, mutationSegmentSize);
                } else if (mutationType == 's') {
                    performShuffleMutation(i, problemSize - 1, mutationSegmentSize);
                }
            } else {
                // Perform crossing
                int parentOneIdx = rand() % populationSize;
                int parentTwoIdx = rand() % populationSize;
                performCrossing(parentOneIdx, parentTwoIdx, i, problemSize - 1, crossingSegmentSize);
            }
        }

        // Step 2: Combine population and offspring
        int combinedSize = 2 * populationSize;
        int** combinedPopulation = new int*[combinedSize];
        int* combinedCosts = new int[combinedSize];
        for (int i = 0; i < populationSize; ++i) {
            combinedPopulation[i] = population[i];
            combinedCosts[i] = calculatePathCost(population[i]);
        }
        for (int i = 0; i < populationSize; ++i) {
            combinedPopulation[populationSize + i] = offspringPopulation[i];
            combinedCosts[populationSize + i] = calculatePathCost(offspringPopulation[i]);
        }

        // Step 3: Select the best chromosomes
        for (int i = 0; i < combinedSize - 1; ++i) {
            for (int j = i + 1; j < combinedSize; ++j) {
                if (combinedCosts[j] < combinedCosts[i]) {
                    std::swap(combinedCosts[i], combinedCosts[j]);
                    std::swap(combinedPopulation[i], combinedPopulation[j]);
                }
            }
        }

        // Step 4: Update the population with the best chromosomes
        for (int i = 0; i < populationSize; ++i) {
            population[i] = combinedPopulation[i];
        }

        // Track the best solution
        if (combinedCosts[0] < bestCost) {
            bestCost = combinedCosts[0];
            if (bestPath) {
                delete[] bestPath;
            }
            bestPath = new int[problemSize - 1];
            for (int i = 0; i < problemSize - 1; ++i) {
                bestPath[i] = combinedPopulation[0][i];
            }
        }

        // Free the memory of combined populations after selection
        for (int i = populationSize; i < combinedSize; ++i) {
            delete[] combinedPopulation[i];
        }
        delete[] combinedPopulation;
        delete[] combinedCosts;

        std::cout << "Iteration " << iteration + 1 << ": Best cost = " << bestCost << std::endl;
    }

    // Output the best solution
    std::cout << "Best solution found:" << std::endl;
    std::cout << "Path: 0 ";
    for (int i = 0; i < problemSize - 1; ++i) {
        std::cout << bestPath[i] << " ";
    }
    std::cout << "0" << std::endl;
    std::cout << "Cost: " << bestCost << std::endl;

    // Free memory for best path
    if (bestPath) {
        delete[] bestPath;
    }
}


int* GeneticAlgorithm::getBestPath() {
    // Allocate memory for the best path
    int* bestPath = new int[problemSize];

    // Ensure problem size is sufficient to form a valid path
    if (problemSize < 3) {
        std::cerr << "Error: problemSize must be at least 3 to form a valid path." << std::endl;
        delete[] bestPath; // Avoid memory leak
        return nullptr;    // Return null to indicate failure
    }

    // Add the starting city (0) to the beginning and end of the path
    bestPath[0] = 0; // Starting city
    for (int i = 1; i < problemSize - 1; ++i) {
        bestPath[i] = population[0][i - 1]; // Fill from the best chromosome in the population
    }
    bestPath[problemSize - 1] = 0; // Return to the starting city


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


void GeneticAlgorithm::printSolution() {
    int* bestPath = getBestPath();
    if (bestPath == nullptr) {
        std::cerr << "Error: Unable to retrieve the best path." << std::endl;
        return;
    }

    int bestCost = getBestSolution();

    std::cout << "Best solution found:" << std::endl;
    std::cout << "Path: ";
    for (int i = 0; i < problemSize; ++i) {
        std::cout << bestPath[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Cost: " << bestCost << std::endl;

    delete[] bestPath;
}


