#ifndef GENETICALGO_H
#define GENETICALGO_H

#include <iostream>
#include <limits>
#include "../../node.h"
#include "../../lib/nlohmann/json.hpp"
#include <string>
#include <algorithms> // for randomising
#include <filesystem>
#include <fstream>


class GeneticAlgorithm{
    public:
        GeneticAlgorithm(const Matrix& matrix, std::string geneticConfigFilePath);
        ~GeneticAlgorithm();
        void runGeneticAlgorithm();
        void const int* getBestPath() const;
        void printBestPath() const;
        void int getBestSolution() const;
    private:
        const Matrix& matrix; // Matrix object initialization
        int problemSize; // size of the problem (total number of genes)
        int** population; // 2D array to store chromosomes
        int** offspring; // 2D to store offspring values
        int* bestPath; // pointer to best path array
        int* selectedPath; // pointer to current path
        int solution; // keeps best solution for the algorithm

        // Parsed values
        int crossingSegmentSize; // parameter to set the size of segment to be crossed over
        int mutationSegmentSize; // parameter to set the size of the segment to be mutated
        std::string mutationType; // parameter to set mutation type (either inversion or shuffle)
        int populationSize; // number of chromosome, taking part in breeding
        int iterationNum; // number of generations 
        int mutationRate; // parameter that sets rete of shuffle or inversion mutation
        int randomRate; // parameter that sets rate of random mutations in offspring

        const int* generateChromosome(int index, ); // Generates random set of cities
        void generatePopulation(int populationSize); // Generates multiple chromosomes
        void generateOffspring(int populationSize, int mutationRate, int randomRate); // Generates offspring containing mutations and crossing
        void performInversionMutation(int currentChromosome); // Takes the part of chromosome (set of cities) and reverses it
        void performShuffleMutation(int currentChromosome); // Takes the part of chromosome and randomly shuffles it
        void performCrossing(); // Takes part of first parent chromosome and puts it to offspring, next feels remain gaps in offspring chromosome with unused genes from another parent chromosome.
        void parseParametersFromJSON(const nlohmann::json& config_json); // Parses required parameters from json config file
        int calculatePathCost(const int* path); // Calculates path cost for current solution 

}


#endif //GENETICALGO_H