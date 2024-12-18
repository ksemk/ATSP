#ifndef BRANCHANDBOUND_H
#define BRANCHANDBOUND_H

#include <vector>
#include <iostream>
#include <limits>
#include "../matrix.h"
#include "../node.h"

/**
 * @class BranchAndBound
 * @brief Class implementing the Branch and Bound algorithm for solving the Traveling Salesman Problem (TSP).
 */
class BranchAndBound {
private:
    const Matrix& matrix; ///< The distance matrix.
    int bestCost; ///< The best cost found during the search.
    std::vector<int> bestPath; ///< The best path found during the search.

    /**
     * @class Subproblem
     * @brief Class representing a subproblem in the Branch and Bound algorithm.
     */
    class Subproblem {
    public:
        std::vector<int> visited; ///< List of visited cities.
        std::vector<int> unvisited; ///< List of unvisited cities.
        int cost; ///< The current cost of the path.
        int lowerBound; ///< The lower bound of the subproblem.

        /**
         * @brief Constructor for the Subproblem class.
         * 
         * @param numCities The number of cities in the problem.
         */
        Subproblem(int numCities);
    };

    /**
     * @brief Calculate the lower bound for a given subproblem.
     * 
     * @param subproblem The subproblem for which to calculate the lower bound.
     * @return int The calculated lower bound.
     */
    int calculateLowerBound(const Subproblem& subproblem) const;

    /**
     * @brief Process a given subproblem.
     * 
     * This function generates new subproblems for each unvisited city and calculates their lower bounds.
     * If the lower bound of a new subproblem is better than the current best cost, the subproblem is added
     * to the queue for further processing.
     * 
     * @param subproblem The subproblem to process.
     */
    void processSubproblem(Subproblem& subproblem);

public:
    /**
     * @brief Constructor for the BranchAndBound class.
     * 
     * @param matrix The distance matrix.
     */
    BranchAndBound(const Matrix& matrix);

    /**
     * @brief Run the Branch and Bound algorithm.
     */
    void runBranchAndBound();

    /**
     * @brief Print the best solution found by the Branch and Bound algorithm.
     */
    void printSolution() const;
};

#endif // BRANCHANDBOUND_H