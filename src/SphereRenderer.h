#pragma once
#include "Sphere.h"
#include <vector>
#include <memory>

class SphereRenderer {
public:
    SphereRenderer();
    ~SphereRenderer();

    // Add a sphere and return its index for later reference
    size_t addSphere(const glm::vec3& position, float radius = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
    size_t addSphere(const Sphere& sphere);

    // Remove sphere by index
    void removeSphere(size_t index);
    void clearSpheres();

    // Update sphere properties by index
    void setSpherePosition(size_t index, const glm::vec3& position);
    void setSphereRadius(size_t index, float radius);
    void setSphereColor(size_t index, const glm::vec4& color);

    // Get sphere by index
    Sphere* getSphere(size_t index);
    size_t getSphereCount() const { return spheres.size(); }

    // Render all spheres
    void render(unsigned int shaderProgram, unsigned int modelLoc, unsigned int colorLoc);

private:
    std::vector<std::unique_ptr<Sphere>> spheres;
    SphereGeometry geometry;
    bool geometryInitialized;
}; 