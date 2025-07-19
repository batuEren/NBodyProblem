#include "SphereRenderer.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

SphereRenderer::SphereRenderer() : geometryInitialized(false) {
    geometry = generateSphereGeometry(32, 32);
    geometryInitialized = true;
}

SphereRenderer::~SphereRenderer() {
    if (geometryInitialized) {
        cleanupSphereGeometry(geometry);
    }
}

size_t SphereRenderer::addSphere(const glm::vec3& position, float radius, const glm::vec4& color) {
    spheres.push_back(std::make_unique<Sphere>(position, radius, color));
    return spheres.size() - 1;
}

size_t SphereRenderer::addSphere(const Sphere& sphere) {
    spheres.push_back(std::make_unique<Sphere>(sphere));
    return spheres.size() - 1;
}

void SphereRenderer::removeSphere(size_t index) {
    if (index < spheres.size()) {
        spheres.erase(spheres.begin() + index);
    }
}

void SphereRenderer::clearSpheres() {
    spheres.clear();
}

void SphereRenderer::setSpherePosition(size_t index, const glm::vec3& position) {
    if (index < spheres.size() && spheres[index]) {
        spheres[index]->setPosition(position);
    }
}

void SphereRenderer::setSphereRadius(size_t index, float radius) {
    if (index < spheres.size() && spheres[index]) {
        spheres[index]->setRadius(radius);
    }
}

void SphereRenderer::setSphereColor(size_t index, const glm::vec4& color) {
    if (index < spheres.size() && spheres[index]) {
        spheres[index]->setColor(color);
    }
}

Sphere* SphereRenderer::getSphere(size_t index) {
    if (index < spheres.size()) {
        return spheres[index].get();
    }
    return nullptr;
}

void SphereRenderer::render(unsigned int shaderProgram, unsigned int modelLoc, unsigned int colorLoc) {
    if (!geometryInitialized) return;

    glUseProgram(shaderProgram);
    glBindVertexArray(geometry.VAO);

    for (const auto& sphere : spheres) {
        if (!sphere) continue;

        glm::mat4 model = sphere->getModelMatrix();
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        const glm::vec4& color = sphere->getColor();
        glUniform4f(colorLoc, color.r, color.g, color.b, color.a);

        glDrawElements(GL_TRIANGLES, geometry.vertexCount, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
} 