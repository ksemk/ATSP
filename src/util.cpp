#include "../include/util.h"


void Util::getStartTime() {
    start = std::chrono::high_resolution_clock::now();
}

void Util::getEndTime() {
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

void Util::elapsedTimeMilliseconds() {
    std::cout << "Elapsed time: " << duration.count() / 1e6 << " milliseconds" << std::endl;
}

void Util::elapsedTimeSeconds() {
    std::cout << "Elapsed time: " << duration.count() / 1e9 << " seconds" << std::endl;
}

void Util::saveResults(std::string algoName, int sampleNum, int size, float time) {

}