#include "Camera.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Camera::Camera()
    : lookAt(0.0f, 0.0f, 0.0f),
    radius(10.0f),
    sphereCoords(glm::vec2(glm::pi<float>() / 2.0f, glm::pi<float>() / 4.0f)) // default top-down
{
    updatePositionFromSphere();
}

void Camera::setLookAt(const glm::vec3& pos) {
    lookAt = pos;
    updatePositionFromSphere();
}

void Camera::setSphereCoords(const glm::vec2& coords) {
    sphereCoords = coords;
    updatePositionFromSphere();
}

void Camera::setRadius(float r) {
    radius = r;
    updatePositionFromSphere();
}

void Camera::shiftSphereCoords(const glm::vec2& delta) {
    sphereCoords += delta;

    // Clamp vertical angle (phi) to avoid poles
    const float epsilon = 0.01f;
    sphereCoords.y = glm::clamp(sphereCoords.y, epsilon, glm::pi<float>() - epsilon);

    updatePositionFromSphere();
}

void Camera::setPosition(const glm::vec3& pos) {
    position = pos;
}

void Camera::move(const glm::vec3& delta) {
    lookAt += delta;
    updatePositionFromSphere();
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

void Camera::updatePositionFromSphere() {
    float theta = sphereCoords.x; // horizontal angle
    float phi = sphereCoords.y;   // vertical angle

    float x = radius * sinf(phi) * cosf(theta);
    float y = radius * cosf(phi);
    float z = radius * sinf(phi) * sinf(theta);

    position = lookAt + glm::vec3(x, y, z);
}