#include "../include/util.h"


void Util::getStartTime() {
    start = std::chrono::high_resolution_clock::now();
}

void Util::getEndTime() {
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

void Util::elapsedTime() {
    std::cout << "Elapsed time: " << duration.count()/10e6 << " milliseconds" << std::endl;
}