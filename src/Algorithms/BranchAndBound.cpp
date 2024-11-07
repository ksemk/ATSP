#include "../../include/Algorithms/BranchAndBound.h"

BranchAndBound::Subproblem::Subproblem(int numCities) : cost(0), lowerBound(0) {
    visited.reserve(numCities);
    visited.push_back(0);  // Start from city 0
    for (int i = 1; i < numCities; ++i) {
        unvisited.push_back(i);
    }
}

BranchAndBound::BranchAndBound(const Matrix& matrix) 
    : matrix(matrix), bestCost(std::numeric_limits<int>::max()) {}

int BranchAndBound::calculateLowerBound(const Subproblem& subproblem) const {
    int bound = subproblem.cost;
    int n = matrix.getSize();

    // For each unvisited city, calculate the minimum cost to enter and exit
    for (int city : subproblem.unvisited) {
        int minOut = std::numeric_limits<int>::max();
        int minIn = std::numeric_limits<int>::max();

        for (int j = 0; j < n; ++j) {
            if (j != city) {
                int costOut = matrix.getCost(city, j);
                int costIn = matrix.getCost(j, city);
                if (costOut < minOut) minOut = costOut;
                if (costIn < minIn) minIn = costIn;
            }
        }
        bound += (minOut + minIn);
    }

    // Divide bound by 2 to avoid overestimation
    return bound / 2;
}

void BranchAndBound::processSubproblem(Subproblem& subproblem) {
    // If all cities are visited, close the tour and update best cost if necessary
    if (subproblem.visited.size() == matrix.getSize()) {
        int tourCost = subproblem.cost + matrix.getCost(subproblem.visited.back(), 0);
        if (tourCost < bestCost) {
            bestCost = tourCost;
            bestPath = subproblem.visited;
            bestPath.push_back(0);  // Return to the start city
        }
        return;
    }

    // Loop through each unvisited city and create a new subproblem for it
    for (size_t i = 0; i < subproblem.unvisited.size(); ++i) {
        int city = subproblem.unvisited[i];
        
        Subproblem newSubproblem = subproblem;  // Duplicate current subproblem
        newSubproblem.visited.push_back(city);
        newSubproblem.cost += matrix.getCost(subproblem.visited.back(), city);

        // Remove the city from unvisited list in the new subproblem
        newSubproblem.unvisited.erase(newSubproblem.unvisited.begin() + i);

        // Calculate the lower bound for this new subproblem
        newSubproblem.lowerBound = calculateLowerBound(newSubproblem);

        // Proceed with the new subproblem if its bound is better than the current best cost
        if (newSubproblem.lowerBound < bestCost) {
            processSubproblem(newSubproblem);
        }
    }
}

void BranchAndBound::runBranchAndBound() {
    Subproblem initial(matrix.getSize());
    initial.lowerBound = calculateLowerBound(initial);
    processSubproblem(initial);
}

void BranchAndBound::printSolution() const {
    std::cout << "Minimum Cost bnb: " << bestCost << std::endl;
    std::cout << "Best Path bnb: ";
    for (int city : bestPath) {
        std::cout << city << " ";
    }
    std::cout << std::endl;
}
