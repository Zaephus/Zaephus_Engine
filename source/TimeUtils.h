
#pragma once

#include <chrono>

class Time {
    public:
        static float deltaTime;

        static void tick();

        static long long int milliseconds();
        static long long int microseconds();
        static long long int nanoseconds();

    private:
        typedef std::chrono::high_resolution_clock tClock;

        static float lastTime;

};