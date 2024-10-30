#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <random> // Include the random library

    // Constructor to initialize the matrix with a given size
    Matrix(int s) : size(s) {
        data.resize(size, std::vector<int>(size, 0)); // Initialize with 0s
    }

    // Function to read matrix from a file
    void readFromFile(const std::string& filename) {
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
                if (!(file >> data[i][j])) {
                    throw std::runtime_error("Error reading matrix data");
                }
            }
        }
    }

    // Function to generate a random matrix with a given size and range
    void generateRandomMatrix(int s, int minValue, int maxValue) {
        if (s < 3 || s > 19) {
            throw std::invalid_argument("Size must be between 3 and 19");
        }
        if (minValue < 1 || maxValue > 9999 || minValue > maxValue) {
            throw std::invalid_argument("Values must be between 1 and 9999, and minValue must be less than or equal to maxValue");
        }

        size = s;
        data.resize(size, std::vector<int>(size, 0));

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(minValue, maxValue);

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (i == j) {
                    data[i][j] = 9999; // No connection to itself
                } else {
                    data[i][j] = dis(gen);
                }
            }
        }
    }

    // Function to display the matrix
    void display() const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                std::cout << std::setw(4) << std::setfill(' ') << data[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    // Getter for matrix size
    int getSize() const {
        return size;
    }
};