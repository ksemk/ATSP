#ifndef UTIL_H
#define UTIL_H

#include "Algorithms/DynamicProgramming.h"
#include "Algorithms/BranchAndBound.h"
#include "Algorithms/BruteForce.h"

#include <chrono>
#include <iostream>
#include <fstream>

class Util {

public:
    void getStartTime();
    void getEndTime();
    void printElapsedTimeMilliseconds();
    void printElapsedTimeSeconds();
    float returnElapsedTimeMilliseconds();
    float returnElapsedTimeSeconds();
    void saveResults(std::string resultPath, std::string algoName, int problemSize, float time);


private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::nanoseconds duration;
};

#endif // UTIL_H
