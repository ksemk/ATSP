#ifndef MAIN_H
#define MAIN_H

#include "util.h"
#include "Algorithms/BranchAndBound.h"
#include "Algorithms/BruteForce.h"
#include "Algorithms/TabuSearch.h"
#include "Algorithms\GeneticAlgo.h"
#include "matrix.h"
#include "node.h"

#include "lib/nlohmann/json.hpp"

#include <fstream>
#include <iostream>
#include <string>

/**
 * @brief Reads the configuration from a JSON object.
 * 
 * This function reads the configuration settings from a JSON object and
 * initializes the global variables used in the program.
 * 
 * @param config_json The JSON object containing the configuration settings.
 */
void readConfig(const nlohmann::json& config_json);

/**
 * @brief Executes multiple algorithms on the given matrix.
 * 
 * This function runs a series of algorithms a specified number of times
 * on the provided matrix. It is useful for benchmarking and comparing
 * the performance of different algorithms on the same data set.
 * 
 * @param numSamples The number of times each algorithm should be executed.
 * @param mat The matrix on which the algorithms will be executed.
 */
void runMultipleAlgorithms(int numSamples, Matrix& mat);

#endif // MAIN_H