#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>

/**
 * @class Matrix
 * @brief A class to represent a matrix and perform various operations on it.
 * 
 * This class provides functionalities to initialize, read, generate, and display a matrix.
 * It also allows fetching the cost between two cities and getting the size of the matrix.
 */
class Matrix {
private:
    int size; ///< The size of the matrix (number of rows/columns).
    int* data; ///< Pointer to the matrix data (flat 1D array to simulate a 2D matrix).

public:
    /**
     * @brief Constructor to initialize the matrix with a given size.
     * 
     * @param s The size of the matrix.
     */
    Matrix(int s);

    /**
     * @brief Destructor to clear the matrix data.
     */
    ~Matrix();

    /**
     * @brief Function to read matrix from a file.
     * 
     * @param filename The name of the file to read the matrix from.
     */
    void readFromFile(const std::string& filename);

    /**
     * @brief Function to generate a random matrix with symmetricity control.
     * 
     * @param s The size of the matrix.
     * @param minValue The minimum value for the matrix elements.
     * @param maxValue The maximum value for the matrix elements.
     * @param symmetricity The level of symmetricity (0 for asymmetric, 1 for symmetric).
     * @param asymRangeMin The minimum range for asymmetric values.
     * @param asymRangeMax The maximum range for asymmetric values.
     */
    void generateRandomMatrix(int s, int minValue, int maxValue, int symmetricity, int asymRangeMin, int asymRangeMax);

    /**
     * @brief Function to get the cost between two cities.
     * 
     * @param i The index of the first city.
     * @param j The index of the second city.
     * @return The cost between the two cities.
     */
    int getCost(int i, int j) const;

    /**
     * @brief Function to get the size of the matrix.
     * 
     * @return The size of the matrix.
     */
    int getSize() const;

    /**
     * @brief Function to display the matrix.
     */
    void display() const;

    /**
     * @brief Function to access elements (row, col) in the matrix.
     * 
     * @param row The row index.
     * @param col The column index.
     * @return The matrix element at (row, col).
     */
    int& at(int row, int col);

    /**
     * @brief Function to access elements (row, col) in the matrix (const version).
     * 
     * @param row The row index.
     * @param col The column index.
     * @return The matrix element at (row, col).
     */
    const int& at(int row, int col) const;
};

#endif // MATRIX_H
