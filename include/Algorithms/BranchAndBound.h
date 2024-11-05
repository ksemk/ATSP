#ifndef BRANCHANDBOUND_H
#define BRANCHANDBOUND_H

#include <vector>
#include <iostream>
#include <limits>
#include "../matrix.h"
#include "../node.h"

class BranchAndBound {
private:
    const Matrix& matrix;
    int bestCost;
    std::vector<int> bestPath;

    class Subproblem {
    public:
        std::vector<int> visited;
        std::vector<int> unvisited;
        int cost;
        int lowerBound;

        Subproblem(int numCities);
    };

    int calculateLowerBound(const Subproblem& subproblem) const;
    void processSubproblem(Subproblem& subproblem);

public:
    BranchAndBound(const Matrix& matrix);
    void runBranchAndBound();
    void printSolution() const;
};

#endif // BRANCHANDBOUND_H