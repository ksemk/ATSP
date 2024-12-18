#include "../../include/Algorithms/BruteForce.h"

// Function to calculate the cost of a given path
int BruteForce::calculatePathCost(const Node& node) const {
    int cost = 0;
    int n = matrix.getSize();
    for (int i = 0; i < n - 1; ++i) {
        cost += matrix.getCost(node.path[i], node.path[i + 1]);
    }
    cost += matrix.getCost(node.path[n - 1], node.path[0]); // Return to the starting city
    return cost;
}

// Function to generate all permutations of the path and calculate the cost
void BruteForce::generatePermutations(Node& node, int left, int right) {
    if (left == right) {
        int currentCost = calculatePathCost(node);
        if (currentCost < bestCost) {
            bestCost = currentCost;
            bestPath = node.path;
        }
    } else {
        for (int i = left; i <= right; ++i) {
            std::swap(node.path[left], node.path[i]);
            generatePermutations(node, left + 1, right);
            std::swap(node.path[left], node.path[i]); // backtrack
        }
    }
}

// Function to run the brute force algorithm
void BruteForce::runBruteForce() {
    int n = matrix.getSize();
    Node node(n);
    for (int i = 0; i < n; ++i) {
        node.path[i] = i;
    }
    generatePermutations(node, 0, n - 1);
}

// Function to print the best solution found
void BruteForce::printSolution() const {
    std::cout << "\nMinimum cost bf: " << bestCost << std::endl;
        std::cout << "Best path bnb: ";
    for (int city : bestPath) {
        std::cout << city << " ";
    }
    std::cout << "0";  // Return to the start city
    std::cout << std::endl;
}