#include "matrix.h"
#define INF -1  // Define infinity as 9999
#include <algorithm> // Include algorithm for std::shuffle

// Constructor to initialize the matrix with a given size
Matrix::Matrix(int s) : size(s) {
    data.resize(size, std::vector<int>(size, 0)); // Initialize with 0s
}

// Function to read matrix from a file
void Matrix::readFromFile(const std::string& filename) {
    clear(); // Clear the existing matrix
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file");
    }

    std::string line;
    int dimension = 0;

    // Read the dimension from the file (look for the DIMENSION line)
    while (std::getline(file, line)) {
        if (line.find("DIMENSION") != std::string::npos) {
            std::istringstream iss(line);
            std::string temp;
            iss >> temp >> dimension; // Read "DIMENSION:" and the number
            break;
        }
    }

    // Resize the matrix based on the dimension
    size = dimension;
    data.resize(size, std::vector<int>(size, 0));

    // Read the matrix values from the file after the EDGE_WEIGHT_SECTION
    while (std::getline(file, line)) {
        if (line.find("EDGE_WEIGHT_SECTION") != std::string::npos) {
            break;
        }
    }

    // Read the matrix data
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            file >> data[i][j];
        }
    }
}

// Function to generate a random matrix with symmetricity control
void Matrix::generateRandomMatrix(int s, int minValue, int maxValue, int symmetricity, int asymRangeMin, int asymRangeMax) {
    clear();
    // Validate input parameters
    if (s < 3 || s > 19) {
        throw std::invalid_argument("Size must be between 3 and 19");
    }
    if (minValue < 1 || maxValue > 9998 || minValue > maxValue) {
        throw std::invalid_argument("Invalid min or max value");
    }
    if (symmetricity < 0 || symmetricity > 100) {
        throw std::invalid_argument("Symmetricity must be between 0 and 100");
    }
    if (asymRangeMin < -maxValue || asymRangeMax > maxValue || asymRangeMin > asymRangeMax) {
        throw std::invalid_argument("Invalid asymmetric range");
    }

    size = s;
    data.resize(size, std::vector<int>(size, 0));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minValue, maxValue);
    std::uniform_int_distribution<> asymDis(asymRangeMin, asymRangeMax);

    // Generate symmetric matrix first
    for (int i = 0; i < s; ++i) {
        for (int j = i + 1; j < s; ++j) {
            int randomValue = dis(gen);
            data[i][j] = randomValue;
            data[j][i] = randomValue; // Make it symmetric
        }
        data[i][i] = INF; // No connection to itself
    }

    // If symmetricity is less than 100, introduce asymmetry
    if (symmetricity < 100) {
        int totalElements = s * (s - 1) / 2; // Number of elements in the upper triangle excluding diagonal
        int elementsToChange = totalElements * (100 - symmetricity) / 100; // Number of elements to change

        std::vector<std::pair<int, int>> indices;
        for (int i = 0; i < s; ++i) {
            for (int j = i + 1; j < s; ++j) {
                indices.emplace_back(i, j);
            }
        }

        std::shuffle(indices.begin(), indices.end(), gen);

        for (int k = 0; k < elementsToChange; ++k) {
            int i = indices[k].first;
            int j = indices[k].second;
            int randomAsymmetry = asymDis(gen);
            data[i][j] += randomAsymmetry; // Modify the upper triangle
        }
    }
}

// Function to display the matrix
void Matrix::display() const {
    for (const auto& row : data) {
        for (const auto& val : row) {
            std::cout << std::setw(5) << val << " ";
        }
        std::cout << std::endl;
    }
}

// Getter for matrix size
int Matrix::getSize() const {
    return size;
}
// Function to clear the memory used by the matrix
void Matrix::clear() {
    size = 0;
    data.clear(); // Clear the vector, releasing the memory

    }

// Getter for matrix data
const std::vector<std::vector<int>>& Matrix::getData() const {
    return data;
}
