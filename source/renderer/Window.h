
#pragma once

#define GLFW_INCLUDE_NONE

#include <map>
#include <string>

#include <GLFW/glfw3.h>

template <typename T>
class Action;

struct Vector2;
struct Vector2Int;

class Window {
    public:
        static Action<void(int, int)> sizeChangedCall;
        static Action<void(int, int)> keyPressedCall;
        static Action<void(int, Vector2)> mousePressedCall;
        static Action<void(Vector2)> cursorMovedCall;

        static Window* activeWindow;

        GLFWwindow* glfwWindow = nullptr;

        Window();
        ~Window();

        // void createWindowed(int _w, int _h, std::string& _title);
        // void createFullscreen(std::string& _title);
        void initialize(size_t _w, size_t _h, const std::string& _title);

        void processCallStack();
        void presentFrame() const;

        void requestInputModeChange(int _mode, int _value);

        [[nodiscard]] Vector2Int getSize() const;
        void setSize(int _w, int _h) const;

        [[nodiscard]] std::string getTitle() const;
        void setTitle(const std::string& _title) const;

        [[nodiscard]] bool shouldClose() const;

        [[nodiscard]] Vector2 screenToClip(const Vector2& _pos) const;

    private:
        static Vector2 cursorPos;
        static Vector2 lastCursorPos;

        std::map<int, int> inputModeStack;

        void createContext(int _w, int _h, const std::string& _title, bool _isFullscreen);

        static void onScreenSizeChange(GLFWwindow* window, int width, int height);
        static void onKeyPressed(GLFWwindow* window, int key, int scanCode, int action, int mods);
        static void onMouseButtonPressed(GLFWwindow* window, int button, int action, int mods);
        static void onCursorMoved(GLFWwindow* window, double x, double y);
};