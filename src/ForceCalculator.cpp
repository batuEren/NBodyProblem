#include "ForceCalculator.h"
#include <glm/glm.hpp>
#include <algorithm>

BruteForceCalculator::BruteForceCalculator(double gravitationalConstant)
    : customG(gravitationalConstant), softeningParameter(0.001) {} // 0.001 AU ≈ 150,000 km (prevents close-encounter instabilities)

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
    glm::vec2 displacement = obj2.getPosition() - obj1.getPosition(); // in AU
    
    // Distance between objects
    double distance = glm::length(displacement); // in AU
    //distance = distance * 149597870700.0; // convert to meters
    //distance = distance * 0.3;
    
    // clamp the distance to avoid singularities when objects are very close
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

glm::vec2 BruteForceCalculator::calculateGravitationalField(const glm::vec2 point, const std::vector<MassObject>& massObjects) const {
    glm::vec2 field = glm::vec2{ 0.0, 0.0 };
    
    const size_t numObjects = massObjects.size();

    // Calculate forces for all obj
    for (size_t i = 0; i < numObjects; ++i) {
        for (size_t j = i + 1; j < numObjects; ++j) {
            glm::vec2 force = calculateGravitationalForce(MassObject(1.0, point), massObjects[j]);
            field += force;
        }
    }

    return field;
}