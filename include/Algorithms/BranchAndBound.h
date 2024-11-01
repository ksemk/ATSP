#ifndef BRANCHANDBOUND_H
#define BRANCHANDBOUND_H

#include <vector>
#include <iostream>
#include "../matrix.h"


class BranchAndBound {
private:
    const Matrix& matrix;  // Reference to the matrix initialized in main
    int bestCost;
    std::vector<int> bestPath;
    

public:
    BranchAndBound(const Matrix& matrix);
    void runBranchAndBound();
    void printSolution() const;

private:
    int calculateLowerBound(Node& node) const;
    // Add any other necessary helper functions here
};

#endif // BRANCHANDBOUND_H