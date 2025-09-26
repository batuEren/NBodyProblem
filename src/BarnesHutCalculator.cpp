#include "BarnesHutCalculator.h"
#include <limits>
#include <algorithm>
#include "QuadTree.h"

BarnesHutCalculator::BarnesHutCalculator(double gravitationalConstant, double theta, double softening)
    : G(gravitationalConstant), theta(theta), softening(softening) {}

std::vector<glm::vec2> BarnesHutCalculator::calculateForces(const std::vector<MassObject>& bodies) const {
    const size_t n = bodies.size();
    std::vector<glm::vec2> forces(n, { 0.0f, 0.0f });
    if (n == 0) return forces;

    // Compute bounding box (AU)
    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();

    for (const auto& b : bodies) {
        glm::vec2 p = b.getPosition();
        minX = std::min(minX, p.x); maxX = std::max(maxX, p.x);
        minY = std::min(minY, p.y); maxY = std::max(maxY, p.y);
    }

    float width = std::max(maxX - minX, 1e-3f);
    float height = std::max(maxY - minY, 1e-3f);
    float half = 0.5f * std::max(width, height);
    glm::vec2 center = { (minX + maxX) * 0.5f, (minY + maxY) * 0.5f };

    QuadNode root({ center, {half, half} });
    for (const auto& b : bodies) root.insert(b);
    root.finalize();

    for (size_t i = 0; i < n; ++i) {
        forces[i] = root.computeForceAt(bodies[i].getPosition(), theta, G, softening) * static_cast<float>(bodies[i].getMass());
    }

    return forces;
    //return std::vector<glm::vec2>(n, { 0.0f, 0.0f });
}