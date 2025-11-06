
#include "Window.h"

#include <iostream>

#include <glad/gl.h>
#include <ZMath.h>

#include "Action.h"

#include <tracy/Tracy.hpp>

Action<void(int, int)> Window::sizeChangedCall      = Action<void(int, int)>();
Action<void(int, int)> Window::keyPressedCall       = Action<void(int, int)>();
Action<void(int, Vector2)> Window::mousePressedCall = Action<void(int, Vector2)>();
Action<void(Vector2)> Window::cursorMovedCall       = Action<void(Vector2)>();

Window* Window::activeWindow = nullptr;

Vector2 Window::cursorPos = Vector2::zero();
Vector2 Window::lastCursorPos = Vector2::zero();

Window::Window() {
    activeWindow = this;
}

Window::~Window() {
    glfwTerminate();
}

void Window::initialize(const size_t _w, const size_t _h, const std::string& _title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    const int width = 1200;//mode->width;
    const int height = 800;//mode->height;

    glfwWindow = glfwCreateWindow(width, height, "Zaephus Renderer", nullptr, nullptr);
    if(glfwWindow == nullptr) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        return;
    }
    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(glfwWindow, onScreenSizeChange);
    glfwSetKeyCallback(glfwWindow, onKeyPressed);
    glfwSetMouseButtonCallback(glfwWindow, onMouseButtonPressed);
    // glfwSetCursorPosCallback(window, onCursorMoved);
    // glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetCursorPos(glfwWindow, 0.0f, 0.0f);

    if(glfwRawMouseMotionSupported()) {
        glfwSetInputMode(glfwWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return;
    }
}

void Window::processCallStack() {
    ZoneScopedNC("Window::ProcessCallStack", 0x0062ff);
    for(auto [mode, value] : inputModeStack) {
        glfwSetInputMode(glfwWindow, mode, value);
    }
}

void Window::presentFrame() const {
    ZoneScopedNC("Window::PresentFrame", 0x0062ff);
    glfwSwapBuffers(glfwWindow);
    glfwPollEvents();
}

void Window::requestInputModeChange(const int _mode, const int _value) {
    inputModeStack[_mode] = _value;
}

Vector2Int Window::getSize() const {
    Vector2Int size;
    glfwGetWindowSize(glfwWindow, &size.x, &size.y);
    return size;
}
void Window::setSize(const int _w, const int _h) const {
    glfwSetWindowSize(glfwWindow, _w, _h);
}

std::string Window::getTitle() const {
    return glfwGetWindowTitle(glfwWindow);
}

void Window::setTitle(const std::string& _title) const {
    glfwSetWindowTitle(glfwWindow, _title.c_str());
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(glfwWindow);
}

Vector2 Window::screenToClip(const Vector2& _pos) const {
    const Vector2Int screenSize = getSize();
    Vector2 pos;
    pos.x = _pos.x / static_cast<float>(screenSize.x);
    pos.y = _pos.y / static_cast<float>(screenSize.y);

    pos.x = 2 * pos.x - 1;
    pos.y = 1 - 2 * pos.y;
    return pos;
}

void Window::createContext(const int _w, const int _h, const std::string& _title, const bool _isFullscreen) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 0;
    int height = 0;
    GLFWmonitor* monitor = nullptr;

    if(_isFullscreen) {
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        width = mode->width;
        height = mode->height;
    }
    else {
        width = _w;
        height = _h;
    }

    glfwWindow = glfwCreateWindow(width, height, _title.c_str(), monitor, nullptr);
    if(glfwWindow == nullptr) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        return;
    }
    glfwMakeContextCurrent(glfwWindow);
    glfwSetFramebufferSizeCallback(glfwWindow, onScreenSizeChange);
    glfwSetKeyCallback(glfwWindow, onKeyPressed);
    glfwSetMouseButtonCallback(glfwWindow, onMouseButtonPressed);
    glfwSetCursorPosCallback(glfwWindow, onCursorMoved);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetCursorPos(window, 0.0f, 0.0f);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return;
    }
}

void Window::onScreenSizeChange(GLFWwindow* window, const int width, const int height) {
    std::cout << width << ", " << height << std::endl;

    glViewport(0, 0, width, height);
    sizeChangedCall.invoke(width, height);
}

void Window::onKeyPressed(GLFWwindow* window, const int key, const int scanCode, const int action, const int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void Window::onMouseButtonPressed(GLFWwindow* window, const int button, const int action, const int mods) {
    if(action == GLFW_PRESS) {
        mousePressedCall.invoke(button, cursorPos);
    }
}

void Window::onCursorMoved(GLFWwindow* window, const double x, const double y) {
    cursorPos = Vector2(static_cast<float>(x), static_cast<float>(y));
    const Vector2 mouseDelta = cursorPos - lastCursorPos;

    lastCursorPos = cursorPos;

    cursorMovedCall.invoke(mouseDelta);
}