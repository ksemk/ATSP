#include "../include/experiment.h"
#include "../include/Algorithms/DynamicProgramming.h"
#include "../include/Algorithms/BranchAndBound.h"
#include "../include/ATSP.h"
#include "matrix.cpp"

int main() {
    Matrix mat(0); // Initialize with size 0, as it will be set from the file

    try {
        // mat.readFromFile("../data/TSPLib_ATSP/br17.atsp");
        // std::cout << "Matrix loaded from file:" << std::endl;
        mat.generateRandomMatrix(5, 1, 10);
        mat.display();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}