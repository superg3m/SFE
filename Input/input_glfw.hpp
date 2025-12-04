#pragma once

#include <input.hpp>
#undef GLFW_INCLUDE_NONE 
#define GLFW_INCLUDE_NONE 
#include <GLFW/glfw3.h>

namespace Input {
    bool GLFW_SETUP(GLFWwindow* window);
    void GLFW_BIND_KEY_CALLBACK(GLFWkeyfun cb);
    void GLFW_BIND_MOUSE_BUTTON_CALLBACK(GLFWmousebuttonfun cb);
    void GLFW_BIND_MOUSE_MOVE_CALLBACK(GLFWcursorposfun cb);
}