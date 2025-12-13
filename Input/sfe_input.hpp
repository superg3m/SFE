#pragma once

#include <set>
#include <cstdint>
#include <functional>

#include "../Core/DataStructure/sfe_ds.hpp"

namespace Input {
    enum KeyState : uint8_t {
        UP       = 0x1,
        PRESSED  = 0x2,
        DOWN     = 0x4,
        RELEASED = 0x8,
    };

    inline constexpr bool operator&(KeyState lhs, KeyState rhs) {
        return static_cast<bool>((static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs)) != 0);
    }

    inline constexpr KeyState operator|(KeyState lhs, KeyState rhs) {
        return static_cast<KeyState>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
    }

    enum KeyCode : uint8_t {
        KEY_A = 'A', KEY_B = 'B', KEY_C = 'C', KEY_D = 'D',
        KEY_E = 'E', KEY_F = 'F', KEY_G = 'G', KEY_H = 'H',
        KEY_I = 'I', KEY_J = 'J', KEY_K = 'K', KEY_L = 'L',
        KEY_M = 'M', KEY_N = 'N', KEY_O = 'O', KEY_P = 'P',
        KEY_Q = 'Q', KEY_R = 'R', KEY_S = 'S', KEY_T = 'T',
        KEY_U = 'U', KEY_V = 'V', KEY_W = 'W', KEY_X = 'X',
        KEY_Y = 'Y', KEY_Z = 'Z',

        KEY_0 = '0', KEY_1 = '1', KEY_2 = '2', KEY_3 = '3',
        KEY_4 = '4', KEY_5 = '5', KEY_6 = '6', KEY_7 = '7',
        KEY_8 = '8', KEY_9 = '9',

        KEY_SPACE = 32, KEY_ENTER = 13, KEY_ESCAPE = 27,
        KEY_TAB = 9, KEY_BACKSPACE = 8,

        KEY_LEFT = 128, KEY_RIGHT, KEY_UP, KEY_DOWN,
        KEY_CTRL, KEY_SHIFT, KEY_ALT,

        KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
        KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,

        MOUSE_BUTTON_LEFT = 200,
        MOUSE_BUTTON_RIGHT,
        MOUSE_BUTTON_MIDDLE
    };

    typedef void (*CALLBACK)();
    struct Profile {
        const char* name;
        CALLBACK callback;
        bool active;
    };

    void Init();
    void Poll();
    void UpdateInputCode(KeyCode code, bool down);
    void UpdateMousePosition(float x, float y);

    bool GetKey(KeyCode code, KeyState state);
    bool GetKeyUp(KeyCode code);
    bool GetKeyPressed(KeyCode code);
    bool GetKeyDown(KeyCode code);
    bool GetKeyReleased(KeyCode code);

    float GetMouseX();
    float GetMouseY();

    void CreateProfile(const char* key, CALLBACK callback, bool active = true);
    void DeleteProfile(const char* key);
    void ToggleProfile(const char* key, bool toggle);
    void EnableProfile(const char* key);
    void DisableProfile(const char* key);

    extern DS::Vector<Profile> profiles;
    extern void* glfw_window_instance;
}