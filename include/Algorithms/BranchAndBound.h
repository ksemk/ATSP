#ifndef BRANCHANDBOUND_H
#define BRANCHANDBOUND_H

#include <vector>
#include <iostream>
#include <limits>
#include "../matrix.h"
#include "../node.h"

class BranchAndBound {
private:
    const Matrix& matrix; // Reference to the matrix initialized in main
    int bestCost;
    std::vector<int> bestPath;
    int calculateLowerBound(const Node& node) const;

public:
    BranchAndBound(const Matrix& matrix) : matrix(matrix), bestCost(INT_MAX) {}
    void runBranchAndBound();
    void printSolution() const;
};

#endif // BRANCHANDBOUND_H