#pragma once

#include <set>
#include <cstdint>
#include <functional>
#include <DataStructure/ds.hpp>

enum class IOD_InputState : uint8_t {
    UP       = 0x1,
    PRESSED  = 0x2,
    DOWN     = 0x4,
    RELEASED = 0x8,
};

#define IOD_INPUT_STATE_HAS_FLAG(value, flag) ((static_cast<uint8_t>(value) & static_cast<uint8_t>(flag)) != 0)

inline constexpr IOD_InputState operator|(IOD_InputState lhs, IOD_InputState rhs) {
    return static_cast<IOD_InputState>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

enum IOD_InputCode : uint8_t {
    IOD_KEY_A = 'A', IOD_KEY_B = 'B', IOD_KEY_C = 'C', IOD_KEY_D = 'D',
    IOD_KEY_E = 'E', IOD_KEY_F = 'F', IOD_KEY_G = 'G', IOD_KEY_H = 'H',
    IOD_KEY_I = 'I', IOD_KEY_J = 'J', IOD_KEY_K = 'K', IOD_KEY_L = 'L',
    IOD_KEY_M = 'M', IOD_KEY_N = 'N', IOD_KEY_O = 'O', IOD_KEY_P = 'P',
    IOD_KEY_Q = 'Q', IOD_KEY_R = 'R', IOD_KEY_S = 'S', IOD_KEY_T = 'T',
    IOD_KEY_U = 'U', IOD_KEY_V = 'V', IOD_KEY_W = 'W', IOD_KEY_X = 'X',
    IOD_KEY_Y = 'Y', IOD_KEY_Z = 'Z',

    IOD_KEY_0 = '0', IOD_KEY_1 = '1', IOD_KEY_2 = '2', IOD_KEY_3 = '3',
    IOD_KEY_4 = '4', IOD_KEY_5 = '5', IOD_KEY_6 = '6', IOD_KEY_7 = '7',
    IOD_KEY_8 = '8', IOD_KEY_9 = '9',

    IOD_KEY_SPACE = 32, IOD_KEY_ENTER = 13, IOD_KEY_ESCAPE = 27,
    IOD_KEY_TAB = 9, IOD_KEY_BACKSPACE = 8,

    IOD_KEY_LEFT = 128, IOD_KEY_RIGHT, IOD_KEY_UP, IOD_KEY_DOWN,
    IOD_KEY_CTRL, IOD_KEY_SHIFT, IOD_KEY_ALT,

    IOD_KEY_F1, IOD_KEY_F2, IOD_KEY_F3, IOD_KEY_F4, IOD_KEY_F5, IOD_KEY_F6,
    IOD_KEY_F7, IOD_KEY_F8, IOD_KEY_F9, IOD_KEY_F10, IOD_KEY_F11, IOD_KEY_F12,

    IOD_MOUSE_BUTTON_LEFT = 200,
    IOD_MOUSE_BUTTON_RIGHT,
    IOD_MOUSE_BUTTON_MIDDLE
};

struct IOD_InputPair {
    IOD_InputCode code;
    IOD_InputState state;
};

struct IOD_InputGroup {
    DS::Vector<IOD_InputCode> codes;
    IOD_InputState state;
};

// NOTE(Jovanni): bool consumed is just has it been consumed/used this frame
typedef void (*IOD_CALLBACK)();
struct IOD_Profile {
    const char* name;
    IOD_CALLBACK callback;
    bool active;
};

struct IOD {
    static void Init();
    static void Poll();
    static void UpdateInputCode(IOD_InputCode code, bool down);
    static void UpdateMousePosition(float x, float y);

    static bool GetKey(IOD_InputCode code, IOD_InputState state);
    static float GetMouseX();
    static float GetMouseY();

    static void CreateProfile(const char* key, IOD_CALLBACK callback);
    static void DeleteProfile(const char* key);
    static void ToggleProfile(const char* key);
    static void EnableProfile(const char* key);
    static void DisableProfile(const char* key);

    static DS::Hashmap<IOD_InputCode, IOD_InputState> input_state;
    static DS::Vector<IOD_Profile> profiles;

    static void* glfw_window_instance;
private:
    static float mouse_x;
    static float mouse_y;
};