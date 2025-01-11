#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H
#define INF 9999  // Define infinity as 9999

#include <iostream>
#include <limits>
#include "../matrix.h"
#include "../node.h"

/**
 * @class BruteForce
 * @brief Class implementing the Brute Force algorithm for solving the Traveling Salesman Problem (TSP).
 */
class BruteForce {
public:
    /**
     * @brief Constructor for the BruteForce class.
     * 
     * @param matrix The distance matrix.
     */
    BruteForce(const Matrix& matrix);

    /**
     * @brief Destructor for the BruteForce class.
     */
    ~BruteForce();

    /**
     * @brief Run the Brute Force algorithm.
     */
    void runBruteForce();

    /**
     * @brief Print the best solution found by the Brute Force algorithm.
     */
    void printSolution() const;

private:
    const Matrix& matrix;  ///< Reference to the matrix initialized in main
    int n;                 ///< Number of cities (size of the matrix)
    int bestCost;          ///< Best cost found
    int* bestPath;         ///< Best path found
    int* currentPath;      ///< Current path during permutation

    /**
     * @brief Calculate the cost of a given path.
     * 
     * @param node The node containing the path.
     * @return int The cost of the path.
     */
    int calculatePathCost(const Node& node) const;

    /**
     * @brief Generate all permutations of the path and calculate the cost.
     * 
     * @param depth The current depth of the permutation.
     */
    void generatePermutations(int depth);

    /**
     * @brief Helper function to swap two integers.
     * 
     * @param a The first integer.
     * @param b The second integer.
     */
    void swap(int& a, int& b);
};

#endif // BRUTEFORCE_H