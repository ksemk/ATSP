#ifndef MAIN_H
#define MAIN_H

#include "util.h"
#include "Algorithms/DynamicProgramming.h"
#include "Algorithms/BranchAndBound.h"
#include "Algorithms/BruteForce.h"
#include "ATSP.h"
#include "matrix.h"
#include "node.h"

#include "lib/nlohmann/json.hpp"

#include <fstream>
#include <iostream>
#include <string>

void readConfig(const nlohmann::json& config_json);
void runMultipleAlgorithms(int numSamples, Matrix& mat);

#endif // MAIN_H