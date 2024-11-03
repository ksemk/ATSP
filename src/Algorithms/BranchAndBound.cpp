#include "../../include/Algorithms/BranchAndBound.h"


struct Node {
    std::vector<int> path;  // Stores the current path of cities
    int level;              // Current level in the tree (depth of recursion)
    int cost;               // Cost of the path so far
    int bound;              // Lower bound for the path

    Node(int numCities) : level(0), cost(0), bound(0) {
        path.resize(numCities, -1); // Initialize path with -1 (unvisited)
    }
};

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
            int minCost = INF;
            for (int j = 0; j < n; ++j) {
                if (!visited[j] && i != j) {
                    minCost = std::min(minCost, matrix.getData()[i][j]);
                }
            }
            if (minCost == INF) {
                // Handle edge case where there's no path from city i
                return INF;
            }
            bound += minCost;
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
    root.bound = calculateLowerBound(root);

    stack.push_back(root);

    while (!stack.empty()) {
        Node current = stack.back();
        stack.pop_back();

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
    std::cout << std::endl;
}