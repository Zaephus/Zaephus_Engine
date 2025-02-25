
#pragma once

#define GLFW_INCLUDE_NONE

#include <string>

#include <GLFW/glfw3.h>

#include "Action.h"

struct Vector2;
struct Vector2Int;

class Window {
    public:
        static Action<void(int, int)> sizeChangedCall;
        static Action<void(int, int)> keyPressedCall;
        static Action<void(int, Vector2)> mousePressedCall;
        static Action<void(Vector2)> cursorMovedCall;

        static Window* activeWindow;

        GLFWwindow* window = nullptr;

        Window();
        ~Window();

        void initialize(size_t _w, size_t _h, const std::string& _title);

        void presentFrame() const;

        Vector2Int getSize() const;
        void setSize(size_t _w, size_t _h) const;

        std::string getTitle() const;
        void setTitle(const std::string& _title) const;

        bool shouldClose() const;

        Vector2 screenToClip(const Vector2& _pos) const;

    private:
        static Vector2 cursorPos;
        static Vector2 lastCursorPos;

        static void onScreenSizeChange(GLFWwindow* window, int width, int height);
        static void onKeyPressed(GLFWwindow* window, int key, int scanCode, int action, int mods);
        static void onMouseButtonPressed(GLFWwindow* window, int button, int action, int mods);
        static void onCursorMoved(GLFWwindow* window, double x, double y);
};