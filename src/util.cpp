#include "../include/util.h"



void Util::getStartTime() {
    start = std::chrono::high_resolution_clock::now();
}

void Util::getEndTime() {
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

void Util::printElapsedTimeMilliseconds() {
    std::cout << "Elapsed time: " << duration.count() / 1e6 << " milliseconds" << std::endl;
}

void Util::printElapsedTimeSeconds() {
    std::cout << "Elapsed time: " << duration.count() / 1e9 << " seconds" << std::endl;
}

float Util::returnElapsedTimeMilliseconds() {
    return duration.count() / 1e6;
}

float Util::returnElapsedTimeSeconds() {
    return duration.count() / 1e9;
}


void Util::saveResults(std::string resultPath, std::string algoName, int problemSize, float time) {
    std::ofstream file(resultPath, std::ios::app);
    if (!file) {
        throw std::runtime_error("Could not open file");
    }
    file << algoName << "," << problemSize << "," << time << std::endl;
    file.close();
}
