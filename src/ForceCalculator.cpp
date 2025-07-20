#include "ForceCalculator.h"
#include <glm/glm.hpp>
#include <algorithm>

BruteForceCalculator::BruteForceCalculator(double gravitationalConstant)
    : customG(gravitationalConstant), softeningParameter(0.01) {}

std::vector<glm::vec2> BruteForceCalculator::calculateForces(const std::vector<MassObject>& massObjects) const {
    const size_t numObjects = massObjects.size();
    std::vector<glm::vec2> forces(numObjects, glm::vec2(0.0, 0.0));
    
    // Calculate forces between all pairs (O(n²))
    for (size_t i = 0; i < numObjects; ++i) {
        for (size_t j = i + 1; j < numObjects; ++j) {
            glm::vec2 force = calculateGravitationalForce(massObjects[i], massObjects[j]);
            
            // Apply Newton's third law (equal and opposite forces)
            forces[i] += force;
            forces[j] -= force;
        }
    }
    
    return forces;
}

glm::vec2 BruteForceCalculator::calculateGravitationalForce(const MassObject& obj1, const MassObject& obj2) const {
    // Vector from obj1 to obj2
    glm::vec2 displacement = obj2.getPosition() - obj1.getPosition();
    
    // Distance between objects
    double distance = glm::length(displacement);
    
    // Apply softening parameter to avoid singularities when objects are very close
    double softenedDistance = std::max(distance, softeningParameter);
    
    // Skip if objects are at the same position (after softening)
    if (softenedDistance < softeningParameter) {
        return glm::vec2(0.0, 0.0);
    }
    
    // Normalize displacement vector
    glm::vec2 direction = displacement / static_cast<float>(distance);
    
    // Calculate gravitational force magnitude: F = G * m1 * m2 / r²
    double forceMagnitude = (customG * obj1.getMass() * obj2.getMass()) / 
                           (softenedDistance * softenedDistance);
    
    // Return force vector (force on obj1 due to obj2)
    return direction * static_cast<float>(forceMagnitude);
} 