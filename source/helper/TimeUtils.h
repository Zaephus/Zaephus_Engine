
#pragma once

class Time {
    public:
        static float deltaTime;

        static void tick();

        static long long int milliseconds();
        static long long int microseconds();
        static long long int nanoseconds();

    private:
        static float lastTime;

};