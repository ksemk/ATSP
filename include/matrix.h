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

    // Constructor to initialize the matrix with a given 2D vector
    Matrix(const std::vector<std::vector<int>>& data);

    // Destructor to clear the matrix data
    ~Matrix();


    // Function to read matrix from a file
    void readFromFile(const std::string& filename);

    // Function to generate a random matrix with symmetricity control
    void generateRandomMatrix(int s, int minValue, int maxValue, int symmetricity, int asymRangeMin, int asymRangeMax);

    // Function to get the cost between two cities
    int getCost(int i, int j) const;

    // Function to get the size of the matrix
    int getSize() const;

    // Function to display the matrix
    void display() const;
};

#endif // MATRIX_H