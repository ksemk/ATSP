#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H
#define INF 9999  // Define infinity as 9999

#include <vector>
#include <iostream>
#include <limits>
#include "../matrix.h"
#include "../node.h"

class BruteForce {
public:
    BruteForce(const Matrix& matrix) 
        : matrix(matrix), n(matrix.getSize()), bestCost(INF) {}

    void runBruteForce();
    void printSolution() const;

private:
    const Matrix& matrix;  // Reference to the matrix initialized in main
    int n;                 // Number of cities (size of the matrix)
    int bestCost;          // Best cost found
    std::vector<int> bestPath; // Best path found

    int calculatePathCost(const Node& node) const;
    void generatePermutations(Node& node, int left, int right);
};

#endif // BRUTEFORCE_H