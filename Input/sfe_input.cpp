#include <algorithm>
#include "sfe_input.hpp"

namespace Input {
    static float mouse_x;
    static float mouse_y;
    static DS::Hashmap<KeyCode, KeyState> input_state;
    
    DS::Vector<Profile> profiles;
    void* glfw_window_instance = nullptr;

    void UpdateInputCode(KeyCode code, bool down) {
        KeyState state = input_state.get(code);
        if (down) {
            KeyState new_state = (state == KeyState::UP || state == KeyState::RELEASED) ? KeyState::PRESSED : KeyState::DOWN;
            input_state.put(code, new_state);
        } else {
            KeyState new_state = (state == KeyState::DOWN || state == KeyState::PRESSED) ? KeyState::RELEASED : KeyState::UP;
            input_state.put(code, new_state); 
        }
    }

    void UpdateMousePosition(float x, float y) {
        mouse_x = x;
        mouse_y = y;
    }

    float GetMouseX() { 
        return mouse_x; 
    }

    float GetMouseY() { 
        return mouse_y; 
    }

    void Init() {
        profiles = DS::Vector<Profile>(1);
        input_state = {
            {KEY_A, KeyState::UP}, {KEY_B, KeyState::UP}, {KEY_C, KeyState::UP},
            {KEY_D, KeyState::UP}, {KEY_E, KeyState::UP}, {KEY_F, KeyState::UP},
            {KEY_G, KeyState::UP}, {KEY_H, KeyState::UP}, {KEY_I, KeyState::UP},
            {KEY_J, KeyState::UP}, {KEY_K, KeyState::UP}, {KEY_L, KeyState::UP},
            {KEY_M, KeyState::UP}, {KEY_N, KeyState::UP}, {KEY_O, KeyState::UP},
            {KEY_P, KeyState::UP}, {KEY_Q, KeyState::UP}, {KEY_R, KeyState::UP},
            {KEY_S, KeyState::UP}, {KEY_T, KeyState::UP}, {KEY_U, KeyState::UP},
            {KEY_V, KeyState::UP}, {KEY_W, KeyState::UP}, {KEY_X, KeyState::UP},
            {KEY_Y, KeyState::UP}, {KEY_Z, KeyState::UP},

            {KEY_0, KeyState::UP}, {KEY_1, KeyState::UP}, {KEY_2, KeyState::UP},
            {KEY_3, KeyState::UP}, {KEY_4, KeyState::UP}, {KEY_5, KeyState::UP},
            {KEY_6, KeyState::UP}, {KEY_7, KeyState::UP}, {KEY_8, KeyState::UP},
            {KEY_9, KeyState::UP},

            {KEY_SPACE, KeyState::UP}, {KEY_ENTER, KeyState::UP},
            {KEY_ESCAPE, KeyState::UP}, {KEY_TAB, KeyState::UP},
            {KEY_BACKSPACE, KeyState::UP},

            {KEY_LEFT, KeyState::UP}, {KEY_RIGHT, KeyState::UP},
            {KEY_UP, KeyState::UP}, {KEY_DOWN, KeyState::UP},
            {KEY_CTRL, KeyState::UP}, {KEY_SHIFT, KeyState::UP},
            {KEY_ALT, KeyState::UP},

            {KEY_F1, KeyState::UP}, {KEY_F2, KeyState::UP},
            {KEY_F3, KeyState::UP}, {KEY_F4, KeyState::UP},
            {KEY_F5, KeyState::UP}, {KEY_F6, KeyState::UP},
            {KEY_F7, KeyState::UP}, {KEY_F8, KeyState::UP},
            {KEY_F9, KeyState::UP}, {KEY_F10, KeyState::UP},
            {KEY_F11, KeyState::UP}, {KEY_F12, KeyState::UP},

            {MOUSE_BUTTON_LEFT, KeyState::UP},
            {MOUSE_BUTTON_RIGHT, KeyState::UP},
            {MOUSE_BUTTON_MIDDLE, KeyState::UP}
        };
    }

    void Poll() {
        for (const auto entry : input_state) {
            if (!entry.filled || entry.dead) continue;

            KeyCode code = entry.key;
            KeyState state = entry.value;

            if (state == KeyState::PRESSED) {
                UpdateInputCode(code, true);
            } else if (state == KeyState::RELEASED) {
                UpdateInputCode(code, false);
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

    bool GetKey(KeyCode code, KeyState state) {
        if (!input_state.has(code)) {
            LOG_WARN("Pressed a key and it is not mapped yet: %c\n", code);
            return false;
        }

        KeyState actual_state = input_state.get(code);
        return state & actual_state;
    }

    bool GetKeyUp(KeyCode code) {
        return GetKey(code, KeyState::UP);
    }

    bool GetKeyPressed(KeyCode code) {
        return GetKey(code, KeyState::PRESSED);
    }

    bool GetKeyDown(KeyCode code) {
        return GetKey(code, KeyState::DOWN);
    }

    bool GetKeyReleased(KeyCode code) {
        return GetKey(code, KeyState::RELEASED);
    }

    void CreateProfile(const char* key, CALLBACK callback, bool active) {
        Profile ret;
        ret.name = key;
        ret.callback = callback;
        ret.active = active;
        profiles.push(ret);
    }

    void DeleteProfile(const char* key) {
        for (int i = 0; i < profiles.count(); i++) {
            Profile profile = profiles[i];
            if (String::Equal(profile.name, key)) {
                profiles.unstableSwapbackRemove(i);
                return;
            }
        }

        LOG_DEBUG("Could not find profile: %s\n", key);
    }

    void ToggleProfile(const char* key, bool toggle) {
        for (int i = 0; i < profiles.count(); i++) {
            Profile* profile = &profiles[i];
            if (String::Equal(profile->name, key)) {
                profile->active = toggle;
                return;
            }
        }

        LOG_DEBUG("Could not find profile: %s\n", key);
    }

    void EnableProfile(const char* key) {
        for (int i = 0; i < profiles.count(); i++) {
            Profile* profile = &profiles[i];
            if (String::Equal(profile->name, key)) {
                profile->active = true;
                return;
            }
        }

        LOG_DEBUG("Could not find profile: %s\n", key);
    }

    void DisableProfile(const char* key) {
        for (int i = 0; i < profiles.count(); i++) {
            Profile* profile = &profiles[i];
            if (String::Equal(profile->name, key)) {
                profile->active = false;
                return;
            }
        }

        LOG_DEBUG("Could not find profile: %s\n", key);
    }
}