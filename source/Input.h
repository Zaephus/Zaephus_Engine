
#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

struct Vector2;

class Input {
    public:
        static void initialize();

        static bool isKeyDown(int _key);
        static void onMousePressed(const int _button, const Vector2 _pos);
};