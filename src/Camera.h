#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    Camera();

    void setPosition(const glm::vec3& pos);  // Manually override computed position
    void setLookAt(const glm::vec3& pos);
    void setSphereCoords(const glm::vec2& coords);
    void setRadius(float r);

    void move(const glm::vec3& delta);       // Moves lookAt + recomputes position
    void movePos(const glm::vec3& delta);    // Moves position directly (not sphere-based)
    void shiftSphereCoords(const glm::vec2& delta); // e.g. dragging to rotate view

    const glm::vec3& getPosition() const;
    const glm::vec3& getLookAt() const;

private:
    void updatePositionFromSphere();

    glm::vec3 position;
    glm::vec3 lookAt;
    float radius;
    glm::vec2 sphereCoords; // (theta, phi)
};

#endif // CAMERA_H