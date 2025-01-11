#include "../include/util.h"

// Function to record the start time
void Util::getStartTime() {
    start = std::chrono::high_resolution_clock::now();
}

// Function to record the end time and calculate the duration
void Util::getEndTime() {
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

// Function to print the elapsed time in milliseconds
void Util::printElapsedTimeMilliseconds() {
    std::cout << "Elapsed time: " << duration.count() / 1e6 << " milliseconds" << std::endl;
}

// Function to print the elapsed time in seconds
void Util::printElapsedTimeSeconds() {
    std::cout << "Elapsed time: " << duration.count() / 1e9 << " seconds" << std::endl;
}

// Function to return the elapsed time in milliseconds
float Util::returnElapsedTimeMilliseconds() {
    return duration.count() / 1e6;
}

// Function to return the elapsed time in seconds
float Util::returnElapsedTimeSeconds() {
    return duration.count() / 1e9;
}

// Function to save the results to a file
void Util::saveResults(std::string resultPath, std::string algoName, int problemSize, float time) {
    std::ofstream file(resultPath, std::ios::app);
    if (!file) {
        throw std::runtime_error("Could not open file");
    }
    file << algoName << "," << problemSize << "," << time << std::endl;
    file.close();
}

// Function to save the results to a file
void Util::saveResultsTabuSearch(std::string resultPath, std::string algoName, int problemSize, float time, int bestCost) {
    std::ofstream file(resultPath, std::ios::app);
    if (!file) {
        throw std::runtime_error("Could not open file");
    }
    file << algoName << "," << problemSize << "," << time << "," << bestCost << std::endl;
    file.close();
}
void Util::saveResultsGA(std::string resultPath, std::string algoName, int problemSize, float time, int bestCost, int populationSize, float mutationRate, float crossoverRate) {
    std::ofstream file(resultPath, std::ios::app);
    if (!file) {
        throw std::runtime_error("Could not open file");
    }
    file << algoName << "," << problemSize << "," << time << "," << bestCost << "," << populationSize << "," << mutationRate << "," << crossoverRate << std::endl;
    file.close();
}