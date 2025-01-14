#include "../../include/Algorithms/TabuSearch.h"


TabuSearch::TabuSearch(const Matrix& matrix, int tabuListSize, int maxIterations)
    : matrix(matrix), tabuListSize(tabuListSize), maxIterations(maxIterations) {
    size = matrix.getSize();
    bestCost = INT_MAX;
    bestPath = new int[size + 1];  // Corrected to size + 1
    currentPath = new int[size + 1];  // Corrected to size + 1

    // Initialize a random path
    currentPath[0] = 0;  // Start at city 0
    bool visited[size] = {false};
    visited[0] = true; // Mark city 0 as visited
    srand(time(nullptr));

    for (int i = 1; i < size; ++i) {
        int city;
        do {
            city = rand() % size; // Pick a random city
        } while (visited[city] || city == 0); // Ensure it's not visited and not city 0

        currentPath[i] = city;
        visited[city] = true; // Mark as visited
    }
    currentPath[size] = 0; // End at city 0

    // Print the initial path
    std::cout << "\nInitial Path: ";
    for (int i = 0; i <= size; i++) {
        std::cout << currentPath[i] << " ";
    }
    std::cout << std::endl;

    initializeTabuList();
}

TabuSearch::~TabuSearch() {
    delete[] bestPath;
    delete[] currentPath;
    for (int i = 0; i < tabuListSize; i++) {
        delete[] tabuList[i];
    }
    delete[] tabuList;
}

void TabuSearch::initializeTabuList() {
    std::cout<< "Iterations number: "<< maxIterations << std::endl;
    std::cout<< "Tabu List Size: "<< tabuListSize << std::endl;
    tabuList = new int*[tabuListSize];
    for (int i = 0; i < tabuListSize; i++) {
        tabuList[i] = new int[2];  // Each move is represented by two integers (i, j)
        tabuList[i][0] = -1;  // Initialize with invalid moves
        tabuList[i][1] = -1;
    }
}

bool TabuSearch::isTabu(int i, int j) {
    for (int k = 0; k < tabuListSize; k++) {
        if ((tabuList[k][0] == i && tabuList[k][1] == j) || (tabuList[k][0] == j && tabuList[k][1] == i)) {
            return true;
        }
    }
    return false;
}

void TabuSearch::updateTabuList(int i, int j) {
    for (int k = 0; k < tabuListSize - 1; k++) {
        tabuList[k][0] = tabuList[k + 1][0];
        tabuList[k][1] = tabuList[k + 1][1];
    }
    tabuList[tabuListSize - 1][0] = i;
    tabuList[tabuListSize - 1][1] = j;
}

void TabuSearch::swapCities(int* path, int i, int j) {
    if (i == 0 || i == size || j == 0 || j == size) {
        return;  // Do not swap the start or end positions
    }
    int temp = path[i];
    path[i] = path[j];
    path[j] = temp;
}

int TabuSearch::calculateCost(const int* path) {
    int totalCost = 0;
    for (int i = 0; i < size; i++) {
        totalCost += matrix.getCost(path[i], path[i + 1]);  // Use getCost method from Matrix
    }
    return totalCost;
}

const int* TabuSearch::runTabuSearch() {
    int currentCost = calculateCost(currentPath);
    bestCost = currentCost;
    memcpy(bestPath, currentPath, (size + 1) * sizeof(int));  // Copy initial path

    int iterations = 0;
    while (iterations < maxIterations) {
        int bestNeighborCost = INT_MAX;
        int bestSwap[2] = {-1, -1};

        int sampleSize = size * (size - 1) / 3;  // Define the neighborhood size

        for (int k = 0; k < sampleSize; k++) {
            int i = rand() % size;
            int j = rand() % size;
            if (i == j) continue;  // Avoid swapping the same city

            // Evaluate the neighbor
            swapCities(currentPath, i, j);
            int neighborCost = calculateCost(currentPath);

            // Check tabu status or aspiration criteria
            if (isTabu(i, j) && neighborCost >= bestCost) {
                swapCities(currentPath, i, j);  // Undo the swap
                continue;
            }

            // Update the best neighbor
            if (neighborCost < bestNeighborCost) {
                bestNeighborCost = neighborCost;
                bestSwap[0] = i;
                bestSwap[1] = j;
            }
            swapCities(currentPath, i, j);  // Undo the swap
        }

        // Apply the best swap if one was found
        if (bestSwap[0] != -1 && bestSwap[1] != -1) {
            swapCities(currentPath, bestSwap[0], bestSwap[1]);
            updateTabuList(bestSwap[0], bestSwap[1]);
            currentCost = bestNeighborCost;

            // Update the global best solution
            if (currentCost < bestCost) {
                bestCost = currentCost;
                memcpy(bestPath, currentPath, (size + 1) * sizeof(int));  // Update best path
            }
        }

        iterations++;
    }

    return bestPath;
}


const int* TabuSearch::getBestPath() const {
    return bestPath;
}

int TabuSearch::getBestCost() const {
    return bestCost;
}

void TabuSearch::printSolutionTabu() const {
    std::cout << "Best Path: ";
    for (int i = 0; i <= size; i++) {  // Corrected to loop through size + 1
        std::cout << bestPath[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Best Cost: " << bestCost << std::endl;
}
