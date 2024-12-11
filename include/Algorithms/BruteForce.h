#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H
#define INF 9999  // Define infinity as 9999

#include <iostream>
#include <limits>
#include "../matrix.h"
#include "../node.h"

class BruteForce {
public:
    BruteForce(const Matrix& matrix);
    ~BruteForce();

    void runBruteForce();
    void printSolution() const;

private:
    const Matrix& matrix;  // Reference to the matrix initialized in main
    int n;                 // Number of cities (size of the matrix)
    int bestCost;          // Best cost found
    int* bestPath;         // Best path found
    int* currentPath;      // Current path during permutation

    int calculatePathCost(const Node& node) const;
    void generatePermutations(int depth);
    void swap(int& a, int& b); // Helper function to swap two integers
};

#endif // BRUTEFORCE_H
