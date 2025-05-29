
#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

class Window;

struct Vector2;
struct Vector2Int;

class Input {
    public:
        static void initialize();
        static void dispose();

        static bool isKeyDown(int _key);
        static bool isKeyUp(int _key);

        static bool isMouseDown(int _button);
        static bool isMouseUp(int _button);

        static void setMouseMode(int _mode);

        static Vector2 getMousePosition();
        static Vector2 getMouseDelta();

    private:
        static Window* window;

        static Vector2 mousePos;
        static Vector2 lastMousePos;

        static void onMousePressed(int _button, Vector2 _pos);
        static void onMouseMoved(GLFWwindow* window, double x, double y);
};