#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>

class Matrix {
private:
    int size;
    std::vector<std::vector<int>> data;

public:
    // Constructor to initialize the matrix with a given size
    Matrix(int s);

    // Function to read matrix from a file
    void readFromFile(const std::string& filename);

    // Function to generate a random matrix with symmetricity control
    void generateRandomMatrix(int s, int minValue, int maxValue, int symmetricity, int asymRangeMin, int asymRangeMax);

    // Function to display the matrix
    void display() const;

    // Getter for matrix size
    int getSize() const;
    
    // Function to clear the memory used by the matrix
    void clear();

};

#endif // MATRIX_H