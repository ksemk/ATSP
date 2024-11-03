#include "../../include/Algorithms/BranchAndBound.h"
#include <climits> // Include for INT_MAX

int BranchAndBound::calculateLowerBound(const Node& node) const {
    int bound = node.cost;
    int n = matrix.getSize();
    std::vector<bool> visited(n, false);

    // Mark cities already visited in the current path
    for (int i = 0; i <= node.level; ++i) {
        visited[node.path[i]] = true;
    }

    // Add minimum outbound edges for each unvisited city
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            int minCost = INT_MAX;
            bool foundMinCost = false;
            for (int j = 0; j < n; ++j) {
                if (visited[j] && matrix.getData()[j][i] != INT_MAX) {
                    minCost = std::min(minCost, matrix.getData()[j][i]);
                    foundMinCost = true;
                }
            }
            if (foundMinCost) {
                bound += minCost;
            } else {
                // If no connected visited city is found, the problem is infeasible
                return INT_MAX;
            }
        }
    }

    return bound;
}

void BranchAndBound::runBranchAndBound() {
    int n = matrix.getSize();
    if (n == 0) {
        std::cerr << "Error: Matrix size is zero." << std::endl;
        return;
    }

    std::vector<Node> stack; // Vector to store nodes in depth-first manner

    Node root(n);
    root.path[0] = 0;  // Start from city 0
    root.level = 0;
    root.cost = 0;
    root.bound = calculateLowerBound(root);

    stack.push_back(root);

    while (!stack.empty()) {
        Node current = stack.back();
        stack.pop_back();

        // // Debug: Print current node details
        // std::cout << "Current Node: ";
        // for (int city : current.path) {
        //     std::cout << city << " ";
        // }
        // std::cout << " | Cost: " << current.cost << " | Bound: " << current.bound << std::endl;

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
                child.cost = current.cost + matrix.getData()[current.path[current.level]][i];

                // Check if this node completes a path
                if (child.level == n - 1) {
                    int totalCost = child.cost + matrix.getData()[i][0];  // Add cost to return to start
                    if (totalCost < bestCost) {
                        bestCost = totalCost;
                        bestPath = child.path;
                        bestPath.push_back(0);  // Complete the path by returning to start

                        // Debug: Print new best path and cost
                        std::cout << "New Best Path: ";
                        for (int city : bestPath) {
                            std::cout << city << " ";
                        }
                        std::cout << " | Cost: " << bestCost << std::endl;
                    }
                } else {
                    // Calculate bound and push to stack if promising
                    child.bound = calculateLowerBound(child);
                    if (child.bound < bestCost) {
                        stack.push_back(child);  // Push onto stack
                    }
                }
            }
        }
    }
}

void BranchAndBound::printSolution() const {
    // Print the best path and cost
    std::cout << "Minimum Cost: " << bestCost << std::endl;
    std::cout << "Best Path: ";
    for (int city : bestPath) {
        std::cout << city << " ";
    }
    std::cout << std::endl;
}