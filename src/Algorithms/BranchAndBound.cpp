#include "../../include/Algorithms/BranchAndBound.h"

#define INF 9999  // Define infinity as 9999

struct Node {
    int path[100];  // Array to store current path of cities, size based on max cities expected
    int level;      // Current level in the tree (depth of recursion)
    int cost;       // Cost of the path so far
    int bound;      // Lower bound for the path

    Node() : level(0), cost(0), bound(0) {
        for (int i = 0; i < 100; ++i) path[i] = -1;  // Initialize path with -1 (unvisited)
    }
};

int BranchAndBound::calculateLowerBound(Node& node) const {
    int bound = node.cost;
    int n = matrix.size;
    bool visited[100] = {false};

    // Mark cities already visited in the current path
    for (int i = 0; i <= node.level; ++i) {
        visited[node.path[i]] = true;
    }

    // Add minimum outbound edges for each unvisited city
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            int minCost = INF;
            for (int j = 0; j < n; ++j) {
                if (!visited[j] && i != j) {
                    minCost = std::min(minCost, matrix.data[i][j]);
                }
            }
            bound += minCost;
        }
    }

    return bound;
}

void BranchAndBound::runBranchAndBound() {
    int n = matrix.size;
    Node stack[1000];      // Stack to store nodes in depth-first manner
    int stackSize = 0;

    Node root;
    root.path[0] = 0;  // Start from city 0
    root.level = 0;
    root.bound = calculateLowerBound(root);

    stack[stackSize++] = root;

    while (stackSize > 0) {
        Node current = stack[--stackSize];  // Pop the top of the stack

        // Only proceed if the bound is less than the best known cost
        if (current.bound < bestCost) {
            for (int i = 0; i < n; ++i) {
                // Check if city i is already visited in the path
                bool visited = false;
                for (int j = 0; j <= current.level; ++j) {
                    if (current.path[j] == i) {
                        visited = true;
                        break;
                    }
                }
                if (visited) continue;

                // Create new node for visiting city i
                Node child = current;
                child.level = current.level + 1;
                child.path[child.level] = i;
                child.cost = current.cost + matrix.data[current.path[current.level]][i];

                // Check if this node completes a path
                if (child.level == n - 1) {
                    int totalCost = child.cost + matrix.data[i][0];  // Add cost to return to start
                    if (totalCost < bestCost) {
                        bestCost = totalCost;
                        for (int j = 0; j < n; ++j) bestPath[j] = child.path[j];
                        bestPath[n] = 0;  // Complete the path by returning to start
                    }
                } else {
                    // Calculate bound and push to stack if promising
                    child.bound = calculateLowerBound(child);
                    if (child.bound < bestCost) {
                        stack[stackSize++] = child;  // Push onto stack
                    }
                }
            }
        }
    }
}

void BranchAndBound::printSolution() const {
    // Print the best path and cost
    std::cout << "Minimum Cost: " << bestCost << std::endl;
    std::cout << "Path: ";
    for (int i = 0; i < matrix.size; ++i) {
        std::cout << bestPath[i] << " -> ";
    }
    std::cout << "0" << std::endl;  // Return to start city
}
