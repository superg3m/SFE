#pragma once

#include <Math/math.hpp>

enum CameraDirection {
    UP,
    DOWN,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float DEFAULTED_YAW         = -90.0f;
const float DEFAULTED_PITCH       =  0.0f;
const float DEFAULTED_SPEED       =  10.0f;
const float DEFAULTED_SENSITIVITY =  0.1f;
const float DEFAULTED_ZOOM        =  45.0f;

struct Camera {
    Math::Vec3 position;
    Math::Vec3 front;
    Math::Vec3 up;
    Math::Vec3 right;
    Math::Vec3 world_up;

    // euler Angles
    float yaw;
    float pitch;

    // camera options
    float mouse_sensitivity;
    float movement_speed;
    float zoom;

    // maybe consider adding like lookat'
    Camera() = default;
    Camera(Math::Vec3 position);
    Camera(float x, float y, float z);
    Math::Mat4 getViewMatrix();
    Math::Mat4 lookat(Math::Vec3 target_position);
    Math::Mat4 lookat(float x, float y, float z);
    void processKeyboard(CameraDirection direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool contrain_pitch = true);
    void processMouseScroll(float yoffset);
    
private:
    void updateCamera();
};