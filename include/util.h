#ifndef UTIL_H
#define UTIL_H

#include <chrono>
#include <iostream>

class Util {

public:

    void getStartTime();

    void getEndTime();

    void elapsedTime();

private:

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

    std::chrono::nanoseconds duration;

};

#endif // UTIL_H
