#ifndef GENETICALGO_H
#define GENETICALGO_H

#include <iostream>
#include <limits>
#include <string>
#include <fstream>
#include <stdexcept>
#include "../lib/nlohmann/json.hpp"
#include "../matrix.h"


class GeneticAlgorithm {
public:
    // Constructor and Destructor
    GeneticAlgorithm(const Matrix& matrix, std::string geneticConfigFilePath);
    ~GeneticAlgorithm();

    // Public Methods
    void runGeneticAlgorithm();         // Main algorithm execution
    int* getBestPath();                 // Get the best path as a dynamically allocated array
    void printSolution();               // Print the best path to the console
    int getBestSolution();              // Get the cost of the best solution
    float getMutationRate();
    float getCrossOverRate();
    int getPopulationSize();
    

private:
    // Problem data and configuration
    const Matrix& matrix;               // Reference to the cost matrix
    int problemSize;                    // Total number of cities
    int** population;                   // 2D array for chromosomes
    int** offspring;                    // 2D array for offspring
    int** offspringPopulation;
    int* bestPath;                      // Array for the best path
    int bestSolution;                   // Cost of the best solution
    std::string geneticConfigFilePath;

    // Genetic algorithm parameters
    int populationSize;                 // Number of chromosomes in the population
    int iterationNum;                   // Number of generations
    int crossingSegmentSize;            // Segment size for crossover
    int mutationSegmentSize;            // Segment size for mutation
    char mutationType;                  // Mutation type ('i' = inversion, 's' = shuffle)
    int mutationRate;                   // Mutation rate (percentage)
    int randomRate;                     // Random mutation rate (percentage)

    // Private Methods
    void parseParametersFromJSON(const nlohmann::json& configJson);                                                    // Parse parameters from JSON
    int* generateChromosome(int chromosomeLength);                                                                     // Generate a random chromosome
    void generatePopulation(int populationSize, int chromosomeLength);                                                 // Initialize the population
    void generateOffspring(int chromosomeLength);                                                                      // Generate offspring with crossover and mutation
    void performInversionMutation(int currentChromosome, int chromosomeLength, int mutationSegmentSize);               // Apply inversion mutation on a chromosome
    void performShuffleMutation(int currentChromosome, int chromosomeLength, int mutationSegmentSize);                 // Apply shuffle mutation on a chromosome
    void performCrossing(int parent1, int parent2, int offspringIndex, int chromosomeLength, int crossingSegmentSize); // Perform crossover on parent chromosomes
    void selection(int chromosomeLength);                                                                              // Select the best chromosomes for the next generation
    int calculatePathCost(const int *path);                                                                            // Calculate the cost of a path 
};

#endif // GENETICALGO_H
