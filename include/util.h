/**
 * @file util.h
 * @brief Utility class for measuring and printing elapsed time, and saving results.
 *
 * This file contains the declaration of the Util class, which provides methods for
 * measuring the start and end time of an operation, calculating the elapsed time,
 * printing the elapsed time in milliseconds and seconds, and saving the results to a file.
 */

#ifndef UTIL_H
#define UTIL_H

#include "Algorithms/BranchAndBound.h"
#include "Algorithms/BruteForce.h"
#include "Algorithms/TabuSearch.h"

#include <chrono>
#include <iostream>
#include <fstream>

/**
 * @class Util
 * @brief A utility class for time measurement and result saving.
 *
 * The Util class provides methods to measure the start and end time of an operation,
 * calculate the elapsed time in milliseconds and seconds, print the elapsed time,
 * and save the results to a specified file.
 */
class Util {

public:
    /**
     * @brief Records the start time of an operation.
     */
    void getStartTime();

    /**
     * @brief Records the end time of an operation.
     */
    void getEndTime();

    /**
     * @brief Prints the elapsed time in milliseconds to the standard output.
     */
    void printElapsedTimeMilliseconds();

    /**
     * @brief Prints the elapsed time in seconds to the standard output.
     */
    void printElapsedTimeSeconds();

    /**
     * @brief Returns the elapsed time in milliseconds.
     * @return Elapsed time in milliseconds.
     */
    float returnElapsedTimeMilliseconds();

    /**
     * @brief Returns the elapsed time in seconds.
     * @return Elapsed time in seconds.
     */
    float returnElapsedTimeSeconds();

    /**
     * @brief Saves the results to a specified file.
     * 
     * @param resultPath The path to the file where results will be saved.
     * @param algoName The name of the algorithm used.
     * @param problemSize The size of the problem.
     * @param time The elapsed time for the operation.
     */
    void saveResults(std::string resultPath, std::string algoName, int problemSize, float time);
    void saveResultsTabuSearch(std::string resultPath, std::string algoName, int problemSize, float time, int bestCost);

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start; ///< Start time of the operation.
    std::chrono::time_point<std::chrono::high_resolution_clock> end; ///< End time of the operation.
    std::chrono::nanoseconds duration; ///< Duration of the operation.
};

#endif // UTIL_H
