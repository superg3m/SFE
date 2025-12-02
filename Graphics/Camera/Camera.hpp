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
const float DEFAULTED_SPEED       =  50.0f;
const float DEFAULTED_SENSITIVITY =  0.1f;
const float DEFAULTED_ZOOM        =  65.0f;

struct Camera {
    Math::Vec3 position;
    Math::Vec3 front;
    Math::Vec3 up;
    Math::Vec3 right;
    Math::Vec3 world_up;

    float yaw = DEFAULTED_YAW;
    float pitch = DEFAULTED_PITCH;
    
    float movement_speed = DEFAULTED_SPEED;
    float mouse_sensitivity = DEFAULTED_SENSITIVITY;
    float zoom = DEFAULTED_ZOOM;

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