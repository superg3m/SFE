#pragma once

#include <IOD.hpp>
#undef GLFW_INCLUDE_NONE 
#define GLFW_INCLUDE_NONE 
#include <GLFW/glfw3.h>

bool IOD_GLFW_SETUP(GLFWwindow* window);
void IOD_GLFW_BIND_KEY_CALLBACK(GLFWkeyfun cb);
void IOD_GLFW_BIND_MOUSE_BUTTON_CALLBACK(GLFWmousebuttonfun cb);
void IOD_GLFW_BIND_MOUSE_MOVE_CALLBACK(GLFWcursorposfun cb);