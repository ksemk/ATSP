#ifndef UTIL_H
#define UTIL_H

#include <chrono>
#include <iostream>

class Util {

public:

    void getStartTime();
    void getEndTime();
    void elapsedTimeMilliseconds();
    void elapsedTimeSeconds();

private:

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::nanoseconds duration;
    void saveResults(std::string algoName, int sampleNum, int size, float time);

};

#endif // UTIL_H
