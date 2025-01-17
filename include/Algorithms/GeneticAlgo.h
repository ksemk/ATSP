/**
 * @file GeneticAlgo.h
 * @brief Header file for the GeneticAlgorithm class, which implements a genetic algorithm for solving the Traveling Salesman Problem (TSP).
 */

#ifndef GENETICALGO_H
#define GENETICALGO_H

#include <iostream>
#include <limits>
#include <string>
#include <fstream>
#include <stdexcept>
#include "../lib/nlohmann/json.hpp"
#include "../matrix.h"

/**
 * @class GeneticAlgorithm
 * @brief Implements a genetic algorithm to solve the Traveling Salesman Problem (TSP).
 *
 * The class leverages a combination of selection, crossover, and mutation techniques to optimize solutions
 * to the TSP. It is designed to work with a given cost matrix and a JSON configuration file for parameter setup.
 */
class GeneticAlgorithm {
public:
    /**
     * @brief Constructs a GeneticAlgorithm object.
     * @param matrix A reference to the cost matrix for the TSP.
     * @param geneticConfigFilePath Path to the JSON configuration file containing algorithm parameters.
     */
    GeneticAlgorithm(const Matrix& matrix, std::string geneticConfigFilePath);

    /**
     * @brief Destructor to clean up dynamically allocated memory.
     */
    ~GeneticAlgorithm();

    // Public Methods

    /**
     * @brief Executes the genetic algorithm to solve the TSP.
     */
    void runGeneticAlgorithm();

    /**
     * @brief Retrieves the best path found by the algorithm.
     * @return A dynamically allocated array representing the best path.
     */
    int* getBestPath();

    /**
     * @brief Prints the best path and its cost to the console.
     */
    void printSolution();

    /**
     * @brief Retrieves the cost of the best solution.
     * @return Cost of the best solution.
     */
    int getBestSolution();

    // Accessor Methods for Algorithm Parameters
    float getMutationRate();
    float getCrossOverRate();
    int getPopulationSize();
    int getRandomRate();
    int getCrossingSegmentSizeRate();
    int getMutationSegmentSizeRate();
    char getMutationType();
    int getIterationNum();
    int getRandomRateNewGen();

private:
    // Problem Data and Configuration

    const Matrix& matrix;               ///< Reference to the cost matrix for the TSP.
    int problemSize;                    ///< Total number of cities in the problem.
    int** population;                   ///< 2D array of chromosomes representing the current population.
    int** offspring;                    ///< 2D array of offspring generated during each generation.
    int** offspringPopulation;          ///< Combined array of current population and offspring.
    int* bestPath;                      ///< Array representing the best path found.
    int bestSolution;                   ///< Cost of the best path found.
    std::string geneticConfigFilePath;  ///< Path to the JSON configuration file.

    // Genetic Algorithm Parameters

    int populationSize;                 ///< Number of chromosomes in the population.
    int iterationNum;                   ///< Number of generations to run the algorithm.
    int crossingSegmentSizeRate;        ///< Percentage of the chromosome used for crossover.
    int mutationSegmentSizeRate;        ///< Percentage of the chromosome used for mutation.
    char mutationType;                  ///< Mutation type ('i' for inversion, 's' for shuffle).
    int mutationRate;                   ///< Percentage of mutations applied to the population.
    int randomRate;                     ///< Percentage of random mutations introduced into the population.
    int randomRateNewGen;               ///< Percentage of new diverse chromosomes added each generation.

    // Private Methods

    /**
     * @brief Parses algorithm parameters from a JSON configuration file.
     * @param configJson JSON object containing algorithm parameters.
     */
    void parseParametersFromJSON(const nlohmann::json& configJson);

    /**
     * @brief Generates a random chromosome.
     * @param chromosomeLength Length of the chromosome.
     * @return Pointer to the generated chromosome array.
     */
    int* generateChromosome(int chromosomeLength);

    /**
     * @brief Initializes the population with random chromosomes.
     * @param populationSize Number of chromosomes in the population.
     * @param chromosomeLength Length of each chromosome.
     */
    void generatePopulation(int populationSize, int chromosomeLength);

    /**
     * @brief Generates offspring using crossover and mutation.
     * @param chromosomeLength Length of each chromosome.
     */
    void generateOffspring(int chromosomeLength);

    /**
     * @brief Applies inversion mutation to a chromosome.
     * @param currentChromosome Index of the chromosome to mutate.
     * @param chromosomeLength Length of the chromosome.
     * @param mutationSegmentSizeRate Percentage of the chromosome to invert.
     */
    void performInversionMutation(int currentChromosome, int chromosomeLength, int mutationSegmentSizeRate);

    /**
     * @brief Applies shuffle mutation to a chromosome.
     * @param currentChromosome Index of the chromosome to mutate.
     * @param chromosomeLength Length of the chromosome.
     * @param mutationSegmentSizeRate Percentage of the chromosome to shuffle.
     */
    void performShuffleMutation(int currentChromosome, int chromosomeLength, int mutationSegmentSizeRate);

    /**
     * @brief Performs crossover between two parent chromosomes to produce offspring.
     * @param parent1 Index of the first parent.
     * @param parent2 Index of the second parent.
     * @param offspringIndex Index of the offspring to create.
     * @param chromosomeLength Length of each chromosome.
     * @param crossingSegmentSizeRate Percentage of the chromosome used for crossover.
     */
    void performCrossing(int parent1, int parent2, int offspringIndex, int chromosomeLength, int crossingSegmentSizeRate);

    /**
     * @brief Selects the best chromosomes for the next generation.
     * @param chromosomeLength Length of each chromosome.
     */
    void selection(int chromosomeLength);

    /**
     * @brief Calculates the cost of a given path.
     * @param path Pointer to the path array.
     * @return Cost of the path.
     */
    int calculatePathCost(const int* path);

    /**
     * @brief Calculates the diversity between two chromosomes.
     * @param chromosome1 First chromosome.
     * @param chromosome2 Second chromosome.
     * @param length Length of the chromosomes.
     * @return Diversity score.
     */
    int calculateDiversity(int* chromosome1, int* chromosome2, int length);
};

#endif // GENETICALGO_H
