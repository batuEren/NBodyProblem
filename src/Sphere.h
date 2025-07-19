#pragma once
#include <glm/glm.hpp>
#include <vector>

struct SphereGeometry {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;
    unsigned int vertexCount;
};

class Sphere {
public:
    Sphere(const glm::vec3& position = glm::vec3(0.0f), 
           float radius = 1.0f, 
           const glm::vec4& color = glm::vec4(1.0f));

    void setPosition(const glm::vec3& pos);
    void setRadius(float r);
    void setColor(const glm::vec4& col);

    const glm::vec3& getPosition() const { return position; }
    float getRadius() const { return radius; }
    const glm::vec4& getColor() const { return color; }

    glm::mat4 getModelMatrix() const;

private:
    glm::vec3 position;
    float radius;
    glm::vec4 color;
};

// Utility functions for sphere geometry
SphereGeometry generateSphereGeometry(int latitudeSegments = 32, int longitudeSegments = 32);
void cleanupSphereGeometry(SphereGeometry& geometry); 