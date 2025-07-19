#include "Camera.h"

Camera::Camera() : position(0.0f, 10.0f, 20.0f) {} //initializes a bit in the air and with a z ofset to get a good look

void Camera::setPosition(const glm::vec3& pos) {
    position = pos;
}

void Camera::setLookAt(const glm::vec3& pos) {
    lookAt = pos;
}

void Camera::move(const glm::vec3& delta) {
    position += delta;
    lookAt += delta;
}

void Camera::movePos(const glm::vec3& delta) {
    position += delta;
}


const glm::vec3& Camera::getPosition() const {
    return position;
}


const glm::vec3& Camera::getLookAt() const {
    return lookAt;
}