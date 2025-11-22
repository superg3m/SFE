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
    Math::Vector3 position;
    Math::Vector3 front;
    Math::Vector3 up;
    Math::Vector3 right;
    Math::Vector3 world_up;

    // euler Angles
    float yaw;
    float pitch;

    // camera options
    float mouse_sensitivity;
    float movement_speed;
    float zoom;

    Camera(Math::Vector3 position);
    Camera(float x, float y, float z);
    Math::Matrix4 getViewMatrix();
    Math::Matrix4 lookat(Math::Vector3 target_position);
    Math::Matrix4 lookat(float x, float y, float z);
    void processKeyboard(CameraDirection direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool contrain_pitch = true);
    void processMouseScroll(float yoffset);
    
private:
    void updateCamera();
};