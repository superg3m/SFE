#include <Input/GLFW_IOD.hpp>

std::unordered_map<int, IOD_InputCode> glfwToInputCode = {
    // Letters
    {GLFW_KEY_A, IOD_KEY_A},
    {GLFW_KEY_B, IOD_KEY_B},
    {GLFW_KEY_C, IOD_KEY_C},
    {GLFW_KEY_D, IOD_KEY_D},
    {GLFW_KEY_E, IOD_KEY_E},
    {GLFW_KEY_F, IOD_KEY_F},
    {GLFW_KEY_G, IOD_KEY_G},
    {GLFW_KEY_H, IOD_KEY_H},
    {GLFW_KEY_I, IOD_KEY_I},
    {GLFW_KEY_J, IOD_KEY_J},
    {GLFW_KEY_K, IOD_KEY_K},
    {GLFW_KEY_L, IOD_KEY_L},
    {GLFW_KEY_M, IOD_KEY_M},
    {GLFW_KEY_N, IOD_KEY_N},
    {GLFW_KEY_O, IOD_KEY_O},
    {GLFW_KEY_P, IOD_KEY_P},
    {GLFW_KEY_Q, IOD_KEY_Q},
    {GLFW_KEY_R, IOD_KEY_R},
    {GLFW_KEY_S, IOD_KEY_S},
    {GLFW_KEY_T, IOD_KEY_T},
    {GLFW_KEY_U, IOD_KEY_U},
    {GLFW_KEY_V, IOD_KEY_V},
    {GLFW_KEY_W, IOD_KEY_W},
    {GLFW_KEY_X, IOD_KEY_X},
    {GLFW_KEY_Y, IOD_KEY_Y},
    {GLFW_KEY_Z, IOD_KEY_Z},

    // Numbers
    {GLFW_KEY_0, IOD_KEY_0},
    {GLFW_KEY_1, IOD_KEY_1},
    {GLFW_KEY_2, IOD_KEY_2},
    {GLFW_KEY_3, IOD_KEY_3},
    {GLFW_KEY_4, IOD_KEY_4},
    {GLFW_KEY_5, IOD_KEY_5},
    {GLFW_KEY_6, IOD_KEY_6},
    {GLFW_KEY_7, IOD_KEY_7},
    {GLFW_KEY_8, IOD_KEY_8},
    {GLFW_KEY_9, IOD_KEY_9},

    // Common control keys
    {GLFW_KEY_SPACE,     IOD_KEY_SPACE},
    {GLFW_KEY_ENTER,     IOD_KEY_ENTER},
    {GLFW_KEY_ESCAPE,    IOD_KEY_ESCAPE},
    {GLFW_KEY_TAB,       IOD_KEY_TAB},
    {GLFW_KEY_BACKSPACE, IOD_KEY_BACKSPACE},

    // Arrow keys
    {GLFW_KEY_LEFT,  IOD_KEY_LEFT},
    {GLFW_KEY_RIGHT, IOD_KEY_RIGHT},
    {GLFW_KEY_UP,    IOD_KEY_UP},
    {GLFW_KEY_DOWN, IOD_KEY_DOWN},

    // Modifier keys
    {GLFW_KEY_LEFT_CONTROL,  IOD_KEY_CTRL},
    {GLFW_KEY_RIGHT_CONTROL, IOD_KEY_CTRL},
    {GLFW_KEY_LEFT_SHIFT,    IOD_KEY_SHIFT},
    {GLFW_KEY_RIGHT_SHIFT,   IOD_KEY_SHIFT},
    {GLFW_KEY_LEFT_ALT,      IOD_KEY_ALT},
    {GLFW_KEY_RIGHT_ALT,     IOD_KEY_ALT},

    // Function keys
    {GLFW_KEY_F1,  IOD_KEY_F1},
    {GLFW_KEY_F2,  IOD_KEY_F2},
    {GLFW_KEY_F3,  IOD_KEY_F3},
    {GLFW_KEY_F4,  IOD_KEY_F4},
    {GLFW_KEY_F5,  IOD_KEY_F5},
    {GLFW_KEY_F6,  IOD_KEY_F6},
    {GLFW_KEY_F7,  IOD_KEY_F7},
    {GLFW_KEY_F8,  IOD_KEY_F8},
    {GLFW_KEY_F9,  IOD_KEY_F9},
    {GLFW_KEY_F10, IOD_KEY_F10},
    {GLFW_KEY_F11, IOD_KEY_F11},
    {GLFW_KEY_F12, IOD_KEY_F12},

    // Mouse buttons
    {GLFW_MOUSE_BUTTON_LEFT,   IOD_MOUSE_BUTTON_LEFT},
    {GLFW_MOUSE_BUTTON_RIGHT,  IOD_MOUSE_BUTTON_RIGHT},
    {GLFW_MOUSE_BUTTON_MIDDLE, IOD_MOUSE_BUTTON_MIDDLE}
};

GLFWkeyfun keyCallback = nullptr;
GLFWmousebuttonfun mouseButtonCallback = nullptr;
GLFWcursorposfun mouseMoveCallback = nullptr;

void IOD_GLFW_BIND_KEY_CALLBACK(GLFWkeyfun cb) {
    keyCallback = cb;
}

void IOD_GLFW_BIND_MOUSE_BUTTON_CALLBACK(GLFWmousebuttonfun cb) {
    mouseButtonCallback = cb;
}

void IOD_GLFW_BIND_MOUSE_MOVE_CALLBACK(GLFWcursorposfun cb) {
    mouseMoveCallback = cb;
}

bool IOD_GLFW_SETUP(GLFWwindow* window) {
    IOD::glfw_window_instance = (void*)window;
    if (!IOD::glfw_window_instance) {
        return false;
    }
    
    glfwSetKeyCallback(window, [](GLFWwindow*, int key, int scancode, int action, int mods) {
        if (!IOD::glfw_window_instance) {
            return;
        }

        if (keyCallback) {
            keyCallback((GLFWwindow*)IOD::glfw_window_instance, key, scancode, action, mods);  
        }

        IOD_InputCode cb_code = glfwToInputCode[key];
        IOD::updateInputCode(cb_code, action != GLFW_RELEASE);
        for (const auto entry : IOD::profiles) {
            if (!entry.filled || entry.dead) continue;

            const char* key = entry.key;
            IOD_Profile* profile = entry.value;

            if (!profile->active) {
                continue;
            }

            for (const auto entry : profile->bindings) {
                if (!entry.filled || entry.dead) continue;

                IOD_InputPair pair = entry.key;
                IOD_InputCode code = pair.code;;
                if (code != cb_code) {
                    continue;
                }

                IOD_CALLBACK callback = entry.value;
                IOD_InputState desired_states = pair.state;
                IOD_InputState actual_state = IOD::input_state.get(code);
                bool has_desired_state_flag = IOD_INPUT_STATE_HAS_FLAG(desired_states, actual_state);
                bool consumed = IOD_INPUT_STATE_HAS_FLAG(actual_state, IOD_InputState::CONSUMED);
                if (!consumed && has_desired_state_flag && callback) {
                    callback(actual_state, consumed);
                    IOD::input_state.put(code, IOD::input_state.get(code) | IOD_InputState::CONSUMED); 
                }
            }
        }
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow*, int button, int action, int mods) {
        if (!IOD::glfw_window_instance) {
            return;
        }

        if (mouseButtonCallback) {
            mouseButtonCallback((GLFWwindow*)IOD::glfw_window_instance, button, action, mods);   
        }

        IOD_InputCode cb_code = glfwToInputCode[button];
        IOD::updateInputCode(cb_code, action != GLFW_RELEASE);
        for (const auto entry : IOD::profiles) {
            if (!entry.filled || entry.dead) continue;

            const char* key = entry.key;
            IOD_Profile* profile = entry.value;

            if (!profile->active) {
                continue;
            }

            for (const auto entry : profile->bindings) {
                if (!entry.filled || entry.dead) continue;

                IOD_InputPair pair = entry.key;
                IOD_InputCode code = pair.code;;
                if (code != cb_code) {
                    continue;
                }

                IOD_CALLBACK callback = entry.value;
                IOD_InputState desired_states = pair.state;
                IOD_InputState actual_state = IOD::input_state.get(code);
                bool has_desired_state_flag = IOD_INPUT_STATE_HAS_FLAG(desired_states, actual_state);
                bool consumed = IOD_INPUT_STATE_HAS_FLAG(actual_state, IOD_InputState::CONSUMED);
                if (!consumed && has_desired_state_flag && callback) {
                    callback(actual_state, consumed);
                    IOD::input_state.put(code, IOD::input_state.get(code) | IOD_InputState::CONSUMED); 
                }
            }
        }
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow*, double xpos, double ypos) {
        if (!IOD::glfw_window_instance) {
            return;
        }

        IOD::updateMousePosition(static_cast<float>(xpos), static_cast<float>(ypos));
        if (mouseMoveCallback) {
            mouseMoveCallback((GLFWwindow*)IOD::glfw_window_instance, xpos, ypos);
        }
    });

    return true;
}
