#include "../include/matrix.h"
#include <algorithm>

Matrix::Matrix(int s) : size(s), data(nullptr) {
    // Allocate memory for a size x size matrix
    data = new int[size * size]; // Allocating a contiguous block of memory for the matrix
    // std::fill(data, data + size * size, 0); // Initialize the matrix with zeros
}

Matrix::~Matrix() {
    // Deallocate the memory
    delete[] data;
}

void Matrix::readFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    // Read the matrix size from the first line
    std::string line;
    if (!std::getline(file, line)) {
        throw std::runtime_error("Error: Could not read the size of the matrix from the file.");
    }
    
    std::stringstream ss(line);
    ss >> size;  // Set the matrix size

    if (size <= 0) {
        throw std::runtime_error("Error: Invalid matrix size.");
    }

    // Allocate memory for the matrix (1D array representation)
    data = new int[size * size];

    int row = 0;
// Read the matrix data from the file
if (file.is_open()) {
    int totalElements = size * size; // Total number of elements in the matrix
    int index = 0; // Index for tracking position in the matrix

    while (index < totalElements && file >> data[index]) {
        // Determine which row we're currently processing
        int row = index / size;
        
        // Modify the first element of the row to 0
        if (index % size == row) {
            data[index] = 0;
        }

        ++index;
    }
}

    file.close();
}


void Matrix::generateRandomMatrix(int s, int minValue, int maxValue, int symmetricity, int asymRangeMin, int asymRangeMax) {
    // Validate inputs
    if (s < 3 || s > 19) {
        throw std::invalid_argument("Size must be between 3 and 19");
    }
    if (minValue < 1 || maxValue > 9998 || minValue > maxValue) {
        throw std::invalid_argument("Invalid min or max value");
    }
    if (symmetricity < 0 || symmetricity > 100) {
        throw std::invalid_argument("Symmetricity must be between 0 and 100");
    }
    if (asymRangeMin < -minValue || asymRangeMax > maxValue || asymRangeMin > asymRangeMax) {
        throw std::invalid_argument("Invalid asymmetric range");
    }

    // Set matrix size and allocate memory for it
    size = s;
    delete[] data; // Deallocate previous memory if any
    data = new int[size * size]; // Allocate new memory

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minValue, maxValue);
    std::uniform_int_distribution<> asymDis(asymRangeMin, asymRangeMax);
    std::uniform_int_distribution<> symDis(0, 99);  // For symmetricity percentage

    // Generate random matrix with symmetricity control
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) { // Only fill upper triangle, handle symmetry later
            int value = dis(gen);

            if (symDis(gen) < symmetricity) {  // Make this pair symmetric
                data[i * size + j] = value;
                data[j * size + i] = value;
            } else {  // Make this pair asymmetric
                data[i * size + j] = value;
                data[j * size + i] = value + asymDis(gen);  // Offset by a random value within asymmetry range
            }
        }
        data[i * size + i] = -1; // Diagonal elements set to -1 (optional)
    }
}

int Matrix::getCost(int i, int j) const {
    return data[i * size + j]; // Access element in flattened matrix
}

int Matrix::getSize() const {
    return size;
}

void Matrix::display() const {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            std::cout << std::setw(5) << data[i * size + j] << " "; // Access element in flattened matrix
        }
        std::cout << std::endl;
    }
}
