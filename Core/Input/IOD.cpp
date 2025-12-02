#include <Input/IOD.hpp>
#include <algorithm>

DS::Hashmap<IOD_InputCode, IOD_InputState> IOD::input_state = {
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

DS::Hashmap<const char*, IOD_Profile*> IOD::profiles = DS::Hashmap<const char*, IOD_Profile*>(1);
void* IOD::glfw_window_instance = nullptr;
float IOD::mouse_x = 0.0f;
float IOD::mouse_y = 0.0f;

void IOD::updateInputCode(IOD_InputCode code, bool down) {
    IOD_InputState state = IOD::input_state.get(code);
    if (down) {
        IOD_InputState new_state = (state == IOD_InputState::UP || state == IOD_InputState::RELEASED) ? IOD_InputState::PRESSED : IOD_InputState::DOWN;
        IOD::input_state.put(code, new_state);
    } else {
        IOD_InputState new_state = (state == IOD_InputState::DOWN || state == IOD_InputState::PRESSED) ? IOD_InputState::RELEASED : IOD_InputState::UP;
        IOD::input_state.put(code, new_state); 
    }
}

void IOD::updateMousePosition(float x, float y) {
    mouse_x = x;
    mouse_y = y;
}

float IOD::getMouseX() { 
    return mouse_x; 
}
float IOD::getMouseY() { 
    return mouse_y; 
}

void IOD::poll() {
    for (const auto entry : IOD::input_state) {
        if (!entry.filled || entry.dead) continue;

        IOD_InputCode code = entry.key;
        IOD_InputState state = entry.value;

        if (state == IOD_InputState::PRESSED) {
            IOD::updateInputCode(code, true);
        } else if (state == IOD_InputState::RELEASED) {
            IOD::updateInputCode(code, false);
        }
    }

    for (const auto entry : IOD::profiles) {
        if (!entry.filled || entry.dead) continue;

        const char* key = entry.key;
        IOD_Profile* profile = entry.value;

        if (!profile->active) {
            continue;
        }

        for (const auto entry : profile->group_bindings) {
            if (!entry.filled || entry.dead) continue;

            IOD_InputGroup key = entry.key;
            IOD_CALLBACK callback = entry.value;

            IOD_InputState desired_states = key.state;
            bool all_good = true;
            for (const auto code : key.codes) {
                if (!entry.filled || entry.dead) continue;

                IOD_InputState actual_state = IOD::input_state.get(code);
                bool has_desired_state_flag = IOD_INPUT_STATE_HAS_FLAG(desired_states, actual_state);
                if (!has_desired_state_flag) {
                    all_good = false;
                }
            } 

            if (!all_good) continue;

            if (callback) {
                // This is not a good way to check the actually state or consumed...
                IOD_InputState actual_state = IOD::input_state.get(key.codes[0]);
                bool consumed = IOD_INPUT_STATE_HAS_FLAG(actual_state, IOD_InputState::CONSUMED);
                callback(actual_state, consumed);
            }

            for (const auto code : key.codes) {
                if (!entry.filled || entry.dead) continue;
                IOD::input_state.put(code, IOD::input_state.get(code) | IOD_InputState::CONSUMED); 
            } 
        }

        for (const auto entry : profile->bindings) {
            if (!entry.filled || entry.dead) continue;

            IOD_InputPair key = entry.key;
            IOD_CALLBACK callback = entry.value;

            IOD_InputState desired_states = key.state;
            IOD_InputState actual_state = IOD::input_state.get(key.code);

            bool has_desired_state_flag = IOD_INPUT_STATE_HAS_FLAG(desired_states, actual_state);
            bool consumed = IOD_INPUT_STATE_HAS_FLAG(actual_state, IOD_InputState::CONSUMED);
            if (has_desired_state_flag && callback) {
                callback(actual_state, consumed);
                IOD::input_state.put(key.code, IOD::input_state.get(key.code) | IOD_InputState::CONSUMED);
            }
        }
    }
}

IOD_InputState IOD::getInputState(IOD_InputCode code) {
    return IOD::input_state.get(code);
}

bool InputGroupEqual(const void* a, byte_t v1_size, const void* b, byte_t v2_size) {
    IOD_InputGroup* g1 = (IOD_InputGroup*)a;
    IOD_InputGroup* g2 = (IOD_InputGroup*)b;

    // Test the vector == vector please!
    return (g1->state == g2->state) && (g1->codes == g2->codes);
}

// NOTE(Jovanni): 
// This is pretty hacky but i'm using a zeroHash to make it do linear probing and that way 
// Theoretically it should be in order? This is so hacky. But if it works its a decent solution
// to the consumed paradigm
IOD_Profile* IOD::createProfile(const char* key) {
    IOD_Profile* ret = new IOD_Profile;
    ret->active = true;
    ret->bindings = DS::Hashmap<IOD_InputPair, IOD_CALLBACK>(Hashing::zeroHash, Memory::equal, 1);
    ret->group_bindings = DS::Hashmap<IOD_InputGroup, IOD_CALLBACK>(Hashing::zeroHash, InputGroupEqual, 1);
    IOD::profiles.put(key, ret);

    return ret;
}

IOD_Profile* IOD::getProfile(const char* key) {
    return IOD::profiles.get(key);
}

void IOD::deleteProfile(const char* key) {
    IOD::profiles.remove(key);
}

void IOD::enableProfile(const char* key) {
    IOD::profiles.get(key)->active = true;
}

void IOD::disableProfile(const char* key) {
    IOD::profiles.get(key)->active = false;
}

void IOD_Profile::bind(IOD_InputCode code, IOD_InputState state, IOD_CALLBACK callack) {
    IOD_InputPair key = IOD_InputPair{code, state};
    this->bindings.put(key, callack);
}

void IOD_Profile::bindGroup(DS::Vector<IOD_InputCode> codes, IOD_InputState state, IOD_CALLBACK callack) {
    IOD_InputGroup key = IOD_InputGroup{codes, state};
    this->group_bindings.put(key, callack);
}

void IOD_Profile::unbind(IOD_InputCode code, IOD_InputState state) {
    IOD_InputPair key = IOD_InputPair{code, state};
    this->bindings.remove(key);
}
    
void IOD_Profile::unbindGroup(DS::Vector<IOD_InputCode> codes, IOD_InputState state) {
    IOD_InputGroup key = IOD_InputGroup{codes, state};
    this->group_bindings.remove(key);
}
    
