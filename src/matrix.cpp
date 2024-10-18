#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <iomanip>

class Matrix {
private:
    int size;
    std::vector<std::vector<int>> data;

public:
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