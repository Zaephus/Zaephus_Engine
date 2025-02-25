
#include "Input.h"

#include <Window.h>

bool Input::isKeyDown(const int _key) {
    return glfwGetKey(Window::activeWindow->window, _key) == GLFW_PRESS;
}