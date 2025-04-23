
#include "TimeUtils.h"

#include <chrono>
#include <GLFW/glfw3.h>

typedef std::chrono::high_resolution_clock tClock;

float Time::deltaTime = 0.0f;
float Time::lastTime = 0.0f;

void Time::tick() {
    deltaTime = static_cast<float>(glfwGetTime()) - lastTime;
    lastTime = static_cast<float>(glfwGetTime());
}

long long int Time::milliseconds() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(tClock::now().time_since_epoch()).count();
}
long long int Time::microseconds() {
    return std::chrono::duration_cast<std::chrono::microseconds>(tClock::now().time_since_epoch()).count();
}
long long int Time::nanoseconds() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(tClock::now().time_since_epoch()).count();
}