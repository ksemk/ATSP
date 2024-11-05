#include "matrix.h"
#define INF 9999  // Define infinity as 9999
#include <algorithm>

// Constructor to initialize the matrix with a given size
Matrix::Matrix(int s) : size(s) {
    data.resize(size, std::vector<int>(size, 0)); // Initialize with 0s
}

// Constructor to initialize the matrix with a given 2D vector
Matrix::Matrix(const std::vector<std::vector<int>>& data) : data(data), size(data.size()) {}

// Function to read matrix from a file
void Matrix::readFromFile(const std::string& filename) {
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

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i == j) {
                data[i][j] = 0;
            } else {
                int value = dis(gen);
                if (symmetricity == 100 || (symmetricity > 0 && dis(gen) % 100 < symmetricity)) {
                    data[i][j] = value;
                    data[j][i] = value;
                } else {
                    data[i][j] = value;
                    data[j][i] = asymDis(gen);
                }
            }
        }
    }
}

// Function to get the cost between two cities
int Matrix::getCost(int i, int j) const {
    return data[i][j];
}

// Function to get the size of the matrix
int Matrix::getSize() const {
    return size;
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