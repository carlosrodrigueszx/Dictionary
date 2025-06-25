#ifndef UTILS_HPP
#define UTILS_HPP

#include <chrono>
#include <string>

class Timer {
    std::chrono::high_resolution_clock::time_point start, end;
public:
    void begin() { 
        start = std::chrono::high_resolution_clock::now(); 
    }
    void stop()  { 
        end = std::chrono::high_resolution_clock::now(); 
    }
    double durationMs() const {
        return std::chrono::duration<double, std::milli>(end - start).count();
    }
};

#endif