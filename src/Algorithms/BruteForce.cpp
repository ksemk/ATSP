#include "../../include/Algorithms/BruteForce.h"
#include <iostream>
#include <climits> // For INT_MAX
#include <cstring> // For std::memcpy

BruteForce::BruteForce(const Matrix& matrix)
    : matrix(matrix), n(matrix.getSize()), bestCost(INT_MAX) {
    // Allocate memory for bestPath and currentPath
    bestPath = new int[n];
    currentPath = new int[n];

    // Initialize paths
    for (int i = 0; i < n; ++i) {
        bestPath[i] = -1;
        currentPath[i] = i; // Start with the default city order
    }
}

BruteForce::~BruteForce() {
    delete[] bestPath;
    delete[] currentPath;
}

void BruteForce::swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int BruteForce::calculatePathCost(const Node& node) const {
    int cost = 0;
    for (int i = 0; i < n - 1; ++i) {
        cost += matrix.getCost(node.path[i], node.path[i + 1]);
    }
    cost += matrix.getCost(node.path[n - 1], node.path[0]); // Return to the starting city
    return cost;
}

void BruteForce::generatePermutations(int depth) {
    if (depth == n) {
        // Calculate the cost of the current permutation
        int currentCost = 0;
        for (int i = 0; i < n - 1; ++i) {
            currentCost += matrix.getCost(currentPath[i], currentPath[i + 1]);
        }
        currentCost += matrix.getCost(currentPath[n - 1], currentPath[0]);

        // Update the best cost and path if needed
        if (currentCost < bestCost) {
            bestCost = currentCost;
            std::memcpy(bestPath, currentPath, n * sizeof(int));
        }
    } else {
        for (int i = depth; i < n; ++i) {
            swap(currentPath[depth], currentPath[i]);
            generatePermutations(depth + 1);
            swap(currentPath[depth], currentPath[i]); // Backtrack
        }
    }
}

void BruteForce::runBruteForce() {
    // Generate all permutations of the cities and find the best path
    generatePermutations(0);
}

void BruteForce::printSolution() const {
    std::cout << "\nMinimum cost (Brute Force): " << bestCost << std::endl;
    std::cout << "Best path (Brute Force): ";
    for (int i = 0; i < n; ++i) {
        std::cout << bestPath[i] << " ";
    }
    std::cout << bestPath[0];  // Return to the start city
    std::cout << std::endl;
}