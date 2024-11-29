#include "StopWatch.hpp"

StopWatch::StopWatch()
{
    this->start_time = std::chrono::high_resolution_clock::now();
}

StopWatch StopWatch::start()
{
    return StopWatch();
}

long StopWatch::lap()
{
    auto current_time = std::chrono::high_resolution_clock::now();
    auto duration = current_time - this->start_time;
    this->start_time = current_time;
    return duration.count();
}