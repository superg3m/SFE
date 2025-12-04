#include <IOD.hpp>
#include <algorithm>

DS::Hashmap<IOD_ICode, IOD_IState> input_state;

DS::Vector<IOD_Profile> IOD::profiles;
void* IOD::glfw_window_instance = nullptr;
float IOD::mouse_x = 0.0f;
float IOD::mouse_y = 0.0f;

void IOD::UpdateInputCode(IOD_ICode code, bool down) {
    IOD_IState state = input_state.get(code);
    if (down) {
        IOD_IState new_state = (state == IOD_IState::UP || state == IOD_IState::RELEASED) ? IOD_IState::PRESSED : IOD_IState::DOWN;
        input_state.put(code, new_state);
    } else {
        IOD_IState new_state = (state == IOD_IState::DOWN || state == IOD_IState::PRESSED) ? IOD_IState::RELEASED : IOD_IState::UP;
        input_state.put(code, new_state); 
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
    IOD::profiles = DS::Vector<IOD_Profile>(1);
    input_state = {
        {IOD_KEY_A, IOD_IState::UP}, {IOD_KEY_B, IOD_IState::UP}, {IOD_KEY_C, IOD_IState::UP},
        {IOD_KEY_D, IOD_IState::UP}, {IOD_KEY_E, IOD_IState::UP}, {IOD_KEY_F, IOD_IState::UP},
        {IOD_KEY_G, IOD_IState::UP}, {IOD_KEY_H, IOD_IState::UP}, {IOD_KEY_I, IOD_IState::UP},
        {IOD_KEY_J, IOD_IState::UP}, {IOD_KEY_K, IOD_IState::UP}, {IOD_KEY_L, IOD_IState::UP},
        {IOD_KEY_M, IOD_IState::UP}, {IOD_KEY_N, IOD_IState::UP}, {IOD_KEY_O, IOD_IState::UP},
        {IOD_KEY_P, IOD_IState::UP}, {IOD_KEY_Q, IOD_IState::UP}, {IOD_KEY_R, IOD_IState::UP},
        {IOD_KEY_S, IOD_IState::UP}, {IOD_KEY_T, IOD_IState::UP}, {IOD_KEY_U, IOD_IState::UP},
        {IOD_KEY_V, IOD_IState::UP}, {IOD_KEY_W, IOD_IState::UP}, {IOD_KEY_X, IOD_IState::UP},
        {IOD_KEY_Y, IOD_IState::UP}, {IOD_KEY_Z, IOD_IState::UP},

        {IOD_KEY_0, IOD_IState::UP}, {IOD_KEY_1, IOD_IState::UP}, {IOD_KEY_2, IOD_IState::UP},
        {IOD_KEY_3, IOD_IState::UP}, {IOD_KEY_4, IOD_IState::UP}, {IOD_KEY_5, IOD_IState::UP},
        {IOD_KEY_6, IOD_IState::UP}, {IOD_KEY_7, IOD_IState::UP}, {IOD_KEY_8, IOD_IState::UP},
        {IOD_KEY_9, IOD_IState::UP},

        {IOD_KEY_SPACE, IOD_IState::UP}, {IOD_KEY_ENTER, IOD_IState::UP},
        {IOD_KEY_ESCAPE, IOD_IState::UP}, {IOD_KEY_TAB, IOD_IState::UP},
        {IOD_KEY_BACKSPACE, IOD_IState::UP},

        {IOD_KEY_LEFT, IOD_IState::UP}, {IOD_KEY_RIGHT, IOD_IState::UP},
        {IOD_KEY_UP, IOD_IState::UP}, {IOD_KEY_DOWN, IOD_IState::UP},
        {IOD_KEY_CTRL, IOD_IState::UP}, {IOD_KEY_SHIFT, IOD_IState::UP},
        {IOD_KEY_ALT, IOD_IState::UP},

        {IOD_KEY_F1, IOD_IState::UP}, {IOD_KEY_F2, IOD_IState::UP},
        {IOD_KEY_F3, IOD_IState::UP}, {IOD_KEY_F4, IOD_IState::UP},
        {IOD_KEY_F5, IOD_IState::UP}, {IOD_KEY_F6, IOD_IState::UP},
        {IOD_KEY_F7, IOD_IState::UP}, {IOD_KEY_F8, IOD_IState::UP},
        {IOD_KEY_F9, IOD_IState::UP}, {IOD_KEY_F10, IOD_IState::UP},
        {IOD_KEY_F11, IOD_IState::UP}, {IOD_KEY_F12, IOD_IState::UP},

        {IOD_MOUSE_BUTTON_LEFT, IOD_IState::UP},
        {IOD_MOUSE_BUTTON_RIGHT, IOD_IState::UP},
        {IOD_MOUSE_BUTTON_MIDDLE, IOD_IState::UP}
    };
}

void IOD::Poll() {
    for (const auto entry : input_state) {
        if (!entry.filled || entry.dead) continue;

        IOD_ICode code = entry.key;
        IOD_IState state = entry.value;

        if (state == IOD_IState::PRESSED) {
            IOD::UpdateInputCode(code, true);
        } else if (state == IOD_IState::RELEASED) {
            IOD::UpdateInputCode(code, false);
        }
    }

    for (const auto profile : profiles) {
        if (!profile.active) {
            continue;
        }

        if (profile.callback) {
            profile.callback();
        }
    }
}

bool IOD::GetKey(IOD_ICode code, IOD_IState state) {
    IOD_IState actual_state = input_state.get(code);
    return state & actual_state;
}

bool IOD::GetKeyUp(IOD_ICode code) {
    IOD_IState actual_state = input_state.get(code);
    return actual_state & IOD_IState::UP;
}

bool IOD::GetKeyPressed(IOD_ICode code) {
    IOD_IState actual_state = input_state.get(code);
    return actual_state & IOD_IState::PRESSED;
}

bool IOD::GetKeyDown(IOD_ICode code) {
    IOD_IState actual_state = input_state.get(code);
    return actual_state & IOD_IState::DOWN;
}

bool IOD::GetKeyReleased(IOD_ICode code) {
    IOD_IState actual_state = input_state.get(code);
    return actual_state & IOD_IState::RELEASED;
}

void IOD::CreateProfile(const char* key, IOD_CALLBACK callback) {
    IOD_Profile ret;
    ret.name = key;
    ret.callback = callback;
    ret.active = true;
    profiles.push(ret);
}

void IOD::DeleteProfile(const char* key) {
    for (int i = 0; i < profiles.count(); i++) {
        IOD_Profile profile = profiles[i];
        if (String::Equal(profile.name, key)) {
            profiles.unstableSwapbackRemove(i);
            return;
        }
    }

    LOG_DEBUG("Could not find profile: %s\n", key);
}

void IOD::ToggleProfile(const char* key) {
    for (int i = 0; i < profiles.count(); i++) {
        IOD_Profile* profile = &profiles[i];
        if (String::Equal(profile->name, key)) {
            profile->active = !profile->active;
            return;
        }
    }

    LOG_DEBUG("Could not find profile: %s\n", key);
}

void IOD::EnableProfile(const char* key) {
    for (int i = 0; i < profiles.count(); i++) {
        IOD_Profile* profile = &profiles[i];
        if (String::Equal(profile->name, key)) {
            profile->active = true;
            return;
        }
    }

    LOG_DEBUG("Could not find profile: %s\n", key);
}

void IOD::DisableProfile(const char* key) {
    for (int i = 0; i < profiles.count(); i++) {
        IOD_Profile* profile = &profiles[i];
        if (String::Equal(profile->name, key)) {
            profile->active = false;
            return;
        }
    }

    LOG_DEBUG("Could not find profile: %s\n", key);
}