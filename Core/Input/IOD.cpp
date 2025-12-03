#include <Input/IOD.hpp>
#include <algorithm>

DS::Hashmap<IOD_InputCode, IOD_InputState> IOD::input_state;
DS::Vector<IOD_Profile> IOD::profiles;
void* IOD::glfw_window_instance = nullptr;
float IOD::mouse_x = 0.0f;
float IOD::mouse_y = 0.0f;

void IOD::UpdateInputCode(IOD_InputCode code, bool down) {
    IOD_InputState state = IOD::input_state.get(code);
    if (down) {
        IOD_InputState new_state = (state == IOD_InputState::UP || state == IOD_InputState::RELEASED) ? IOD_InputState::PRESSED : IOD_InputState::DOWN;
        IOD::input_state.put(code, new_state);
    } else {
        IOD_InputState new_state = (state == IOD_InputState::DOWN || state == IOD_InputState::PRESSED) ? IOD_InputState::RELEASED : IOD_InputState::UP;
        IOD::input_state.put(code, new_state); 
    }
}

void IOD::UpdateMousePosition(float x, float y) {
    IOD::mouse_x = x;
    IOD::mouse_y = y;
}

float IOD::GetMouseX() { 
    return mouse_x; 
}
float IOD::GetMouseY() { 
    return mouse_y; 
}

void IOD::Init() {
   IOD::input_state = {
        {IOD_KEY_A, IOD_InputState::UP}, {IOD_KEY_B, IOD_InputState::UP}, {IOD_KEY_C, IOD_InputState::UP},
        {IOD_KEY_D, IOD_InputState::UP}, {IOD_KEY_E, IOD_InputState::UP}, {IOD_KEY_F, IOD_InputState::UP},
        {IOD_KEY_G, IOD_InputState::UP}, {IOD_KEY_H, IOD_InputState::UP}, {IOD_KEY_I, IOD_InputState::UP},
        {IOD_KEY_J, IOD_InputState::UP}, {IOD_KEY_K, IOD_InputState::UP}, {IOD_KEY_L, IOD_InputState::UP},
        {IOD_KEY_M, IOD_InputState::UP}, {IOD_KEY_N, IOD_InputState::UP}, {IOD_KEY_O, IOD_InputState::UP},
        {IOD_KEY_P, IOD_InputState::UP}, {IOD_KEY_Q, IOD_InputState::UP}, {IOD_KEY_R, IOD_InputState::UP},
        {IOD_KEY_S, IOD_InputState::UP}, {IOD_KEY_T, IOD_InputState::UP}, {IOD_KEY_U, IOD_InputState::UP},
        {IOD_KEY_V, IOD_InputState::UP}, {IOD_KEY_W, IOD_InputState::UP}, {IOD_KEY_X, IOD_InputState::UP},
        {IOD_KEY_Y, IOD_InputState::UP}, {IOD_KEY_Z, IOD_InputState::UP},

        {IOD_KEY_0, IOD_InputState::UP}, {IOD_KEY_1, IOD_InputState::UP}, {IOD_KEY_2, IOD_InputState::UP},
        {IOD_KEY_3, IOD_InputState::UP}, {IOD_KEY_4, IOD_InputState::UP}, {IOD_KEY_5, IOD_InputState::UP},
        {IOD_KEY_6, IOD_InputState::UP}, {IOD_KEY_7, IOD_InputState::UP}, {IOD_KEY_8, IOD_InputState::UP},
        {IOD_KEY_9, IOD_InputState::UP},

        {IOD_KEY_SPACE, IOD_InputState::UP}, {IOD_KEY_ENTER, IOD_InputState::UP},
        {IOD_KEY_ESCAPE, IOD_InputState::UP}, {IOD_KEY_TAB, IOD_InputState::UP},
        {IOD_KEY_BACKSPACE, IOD_InputState::UP},

        {IOD_KEY_LEFT, IOD_InputState::UP}, {IOD_KEY_RIGHT, IOD_InputState::UP},
        {IOD_KEY_UP, IOD_InputState::UP}, {IOD_KEY_DOWN, IOD_InputState::UP},
        {IOD_KEY_CTRL, IOD_InputState::UP}, {IOD_KEY_SHIFT, IOD_InputState::UP},
        {IOD_KEY_ALT, IOD_InputState::UP},

        {IOD_KEY_F1, IOD_InputState::UP}, {IOD_KEY_F2, IOD_InputState::UP},
        {IOD_KEY_F3, IOD_InputState::UP}, {IOD_KEY_F4, IOD_InputState::UP},
        {IOD_KEY_F5, IOD_InputState::UP}, {IOD_KEY_F6, IOD_InputState::UP},
        {IOD_KEY_F7, IOD_InputState::UP}, {IOD_KEY_F8, IOD_InputState::UP},
        {IOD_KEY_F9, IOD_InputState::UP}, {IOD_KEY_F10, IOD_InputState::UP},
        {IOD_KEY_F11, IOD_InputState::UP}, {IOD_KEY_F12, IOD_InputState::UP},

        {IOD_MOUSE_BUTTON_LEFT, IOD_InputState::UP},
        {IOD_MOUSE_BUTTON_RIGHT, IOD_InputState::UP},
        {IOD_MOUSE_BUTTON_MIDDLE, IOD_InputState::UP}
    };

    IOD::profiles = DS::Vector<IOD_Profile>(1);
}

void IOD::Poll() {
    for (const auto entry : IOD::input_state) {
        if (!entry.filled || entry.dead) continue;

        IOD_InputCode code = entry.key;
        IOD_InputState state = entry.value;

        if (state == IOD_InputState::PRESSED) {
            IOD::UpdateInputCode(code, true);
        } else if (state == IOD_InputState::RELEASED) {
            IOD::UpdateInputCode(code, false);
        }
    }

    for (const auto profile : IOD::profiles) {
        if (!profile.active) {
            continue;
        }

        if (profile.callback) {
            profile.callback();
        }
    }
}

bool IOD::GetKey(IOD_InputCode code, IOD_InputState state) {
    IOD_InputState actual_state = IOD::input_state.get(code);
    return IOD_INPUT_STATE_HAS_FLAG(state, actual_state);
}

bool InputGroupEqual(const void* a, byte_t v1_size, const void* b, byte_t v2_size) {
    IOD_InputGroup* g1 = (IOD_InputGroup*)a;
    IOD_InputGroup* g2 = (IOD_InputGroup*)b;

    // Test the vector == vector please!
    return (g1->state == g2->state) && (g1->codes == g2->codes);
}

void IOD::CreateProfile(const char* key, IOD_CALLBACK callback) {
    IOD_Profile ret;
    ret.name = key;
    ret.callback = callback;
    ret.active = true;
    IOD::profiles.push(ret);
}

void IOD::DeleteProfile(const char* key) {
    for (int i = 0; i < IOD::profiles.count(); i++) {
        IOD_Profile profile = IOD::profiles[i];
        if (String::equal(profile.name, key)) {
            IOD::profiles.unstableSwapbackRemove(i);
            return;
        }
    }

    LOG_DEBUG("Could not find profile: %s\n", key);
}

void IOD::ToggleProfile(const char* key) {
    for (int i = 0; i < IOD::profiles.count(); i++) {
        IOD_Profile* profile = &IOD::profiles[i];
        if (String::equal(profile->name, key)) {
            profile->active = !profile->active;
            return;
        }
    }

    LOG_DEBUG("Could not find profile: %s\n", key);
}

void IOD::EnableProfile(const char* key) {
    for (int i = 0; i < IOD::profiles.count(); i++) {
        IOD_Profile* profile = &IOD::profiles[i];
        if (String::equal(profile->name, key)) {
            profile->active = true;
            return;
        }
    }

    LOG_DEBUG("Could not find profile: %s\n", key);
}

void IOD::DisableProfile(const char* key) {
    for (int i = 0; i < IOD::profiles.count(); i++) {
        IOD_Profile* profile = &IOD::profiles[i];
        if (String::equal(profile->name, key)) {
            profile->active = false;
            return;
        }
    }

    LOG_DEBUG("Could not find profile: %s\n", key);
}