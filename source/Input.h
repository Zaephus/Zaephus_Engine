
#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

struct Vector2;

class Input {
    public:
        static void initialize();

        static bool isKeyDown(int _key);
        static bool isMouseDown(int _button);

        static Vector2 getMousePosition();

        static void onMousePressed(int _button, Vector2 _pos);
};