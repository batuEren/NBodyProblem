#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    Camera();

    void setPosition(const glm::vec3& pos);
    void setLookAt(const glm::vec3& pos);
    void move(const glm::vec3& delta);
    void movePos(const glm::vec3& delta);
    const glm::vec3& getPosition() const;
    const glm::vec3& getLookAt() const;

private:
    glm::vec3 position;
    glm::vec3 lookAt;
};

#endif // CAMERA_H
