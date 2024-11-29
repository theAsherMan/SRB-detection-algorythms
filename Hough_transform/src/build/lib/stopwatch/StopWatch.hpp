#pragma once

#include <chrono>

class StopWatch
{
private:
    std::chrono::_V2::system_clock::time_point start_time;
    StopWatch();
public:
    static StopWatch start();
    long lap();
};