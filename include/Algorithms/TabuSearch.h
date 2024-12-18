#ifndef TABUSEARCH_H
#define TABUSEARCH_H

#include <iostream>
#include "../matrix.h"
#include "../node.h"
#include <limits>
#include <ctime>
#include <random>

/**
 * @class TabuSearch
 * @brief Class implementing the Tabu Search algorithm for solving the Traveling Salesman Problem (TSP).
 */
class TabuSearch {
public:
    /**
     * @brief Constructor for the TabuSearch class.
     * 
     * @param matrix The distance matrix.
     * @param tabuListSize The size of the tabu list.
     * @param maxIterations The maximum number of iterations for the search.
     */
    TabuSearch(const Matrix& matrix, int tabuListSize, int maxIterations);

    /**
     * @brief Destructor for the TabuSearch class.
     */
    ~TabuSearch();

    /**
     * @brief Run the Tabu Search algorithm and return the best solution found.
     * 
     * @return const int* Pointer to the best path found.
     */
    const int* runTabuSearch();

    /**
     * @brief Print the best solution found by the Tabu Search algorithm.
     */
    void printSolutionTabu() const;

    /**
     * @brief Get the best path found by the Tabu Search algorithm.
     * 
     * @return const int* Pointer to the best path found.
     */
    const int* getBestPath() const;

    /**
     * @brief Get the cost of the best path found by the Tabu Search algorithm.
     * 
     * @return int The cost of the best path found.
     */
    int getBestCost() const;

private:
    const Matrix& matrix;       ///< The distance matrix.
    int size;                   ///< Number of cities (size of the matrix).
    int bestCost;               ///< Best cost found during the search.
    int* bestPath;              ///< Best path found during the search.
    int* currentPath;           ///< Current path being evaluated.
    int tabuListSize;           ///< Size of the tabu list.
    int** tabuList;             ///< The tabu list to store moves.
    int maxIterations;          ///< Max number of iterations for the search.

    /**
     * @brief Initialize the tabu list.
     */
    void initializeTabuList();

    /**
     * @brief Check if a move is tabu.
     * 
     * @param i The first city in the move.
     * @param j The second city in the move.
     * @return bool True if the move is tabu, false otherwise.
     */
    bool isTabu(int i, int j);

    /**
     * @brief Update the tabu list with a new move.
     * 
     * @param i The first city in the move.
     * @param j The second city in the move.
     */
    void updateTabuList(int i, int j);

    /**
     * @brief Swap two cities in the path.
     * 
     * @param path The path in which to swap cities.
     * @param i The first city to swap.
     * @param j The second city to swap.
     */
    void swapCities(int* path, int i, int j);

    /**
     * @brief Calculate the cost of a given path.
     * 
     * @param path The path for which to calculate the cost.
     * @return int The cost of the path.
     */
    int calculateCost(const int* path);
};

#endif // TABUSEARCH_H