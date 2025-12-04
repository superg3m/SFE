#include <Camera/camera.hpp>

void Camera::updateCamera() {
    Math::Vec3 new_front = Math::Vec3::Euler(this->yaw, this->pitch);
    this->front = new_front.normalize();
    this->right = Math::Vec3::Cross(this->front, this->world_up).normalize();
    this->up    = Math::Vec3::Cross(this->right, this->front).normalize();
}

Camera::Camera(Math::Vec3 position) {
    this->position = position;

    this->world_up = Math::Vec3(0, 1, 0);
    this->up = Math::Vec3(0, 1, 0);

    this->mouse_sensitivity = DEFAULTED_SENSITIVITY;
    this->movement_speed = DEFAULTED_SPEED;
    this->zoom = DEFAULTED_ZOOM;
    this->yaw = DEFAULTED_YAW;
    this->pitch = DEFAULTED_PITCH;

    this->updateCamera();
}

Camera::Camera(float x, float y, float z) {
    this->position = Math::Vec3(x, y, z);

    this->world_up = Math::Vec3(0, 1, 0);
    this->up = Math::Vec3(0, 1, 0);

    this->mouse_sensitivity = DEFAULTED_SENSITIVITY;
    this->movement_speed = DEFAULTED_SPEED;
    this->zoom = DEFAULTED_ZOOM;
    this->yaw = DEFAULTED_YAW;
    this->pitch = DEFAULTED_PITCH;

    this->updateCamera();
}


Math::Mat4 Camera::lookat(Math::Vec3 target_position) {
    return Math::Mat4::Lookat(this->position, target_position, this->up);
}

Math::Mat4 Camera::lookat(float x, float y, float z) {
    return Math::Mat4::Lookat(this->position, Math::Vec3(x, y, z), this->up);
}

Math::Mat4 Camera::getViewMatrix() {
    return Math::Mat4::Lookat(this->position, this->position + this->front, this->up);
}

void Camera::processKeyboard(CameraDirection direction, float deltaTime) {
    float velocity = this->movement_speed * deltaTime;

    if (direction == UP) {
        this->position = this->position + this->world_up.scale(velocity);
    }

    if (direction == DOWN) {
        this->position = this->position + this->world_up.scale(-velocity);
    }

    if (direction == FORWARD) {
        this->position = this->position + this->front.scale(velocity);
    }
    
    if (direction == BACKWARD) {
        this->position = this->position + this->front.scale(-velocity);
    }
    
    if (direction == LEFT) {
        this->position = this->position + this->right.scale(-velocity);
    }
    
    if (direction == RIGHT) {
        this->position = this->position + this->right.scale(velocity);
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool contrain_pitch) {
    xoffset *= this->mouse_sensitivity;
    yoffset *= this->mouse_sensitivity;

    this->yaw   += xoffset;
    this->pitch += yoffset;

    if (contrain_pitch) {
        if (this->pitch > 89.0f) {
            this->pitch = 89.0f;
        } else if (this->pitch < -89.0f) {
            this->pitch = -89.0f;
        }
    }

    this->updateCamera();
}

void Camera::processMouseScroll(float yoffset) {
    this->zoom -= yoffset;
    if (this->zoom < 1.0f) {
        this->zoom = 1.0f;
    }

    if (this->zoom > 45.0f) {
        this->zoom = 45.0f;
    }
}