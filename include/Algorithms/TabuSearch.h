#ifndef TABUSEARCH_H
#define TABUSEARCH_H

#include <iostream>
#include "../matrix.h"
#include "../node.h"
#include <limits>
#include <ctime>
#include <random>



class TabuSearch {
public:
    TabuSearch(const Matrix& matrix, int tabuListSize, int maxIterations);
    ~TabuSearch();

    const int* runTabuSearch();  // Run the Tabu Search and return the best solution
    void printSolutionTabu() const;  // Print the best solution found

private:
    const Matrix& matrix;
    int size;                   // Number of cities (size of the matrix)
    int bestCost;               // Best cost found during search
    int* bestPath;              // Best path found during search
    int* currentPath;           // Current path being evaluated
    int tabuListSize;           // Size of the tabu list
    int** tabuList;             // The tabu list to store moves
    int maxIterations;          // Max number of iterations for the search

    void initializeTabuList();  // Initialize the tabu list
    bool isTabu(int i, int j);  // Check if a move is tabu
    void updateTabuList(int i, int j);  // Update the tabu list
    void swapCities(int* path, int i, int j);  // Swap cities in the path
    int calculateCost(const int* path);  // Calculate the cost of a path
};

#endif // TABUSEARCH_H