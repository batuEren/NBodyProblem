#include "MassObjectTracker.h"
#include "ForceCalculator.h"
#include "Integrator.h"
#include <glm/glm.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>

MassObjectTracker::MassObjectTracker() {
    physicsEngine = std::make_unique<PhysicsEngine>();
}

void MassObjectTracker::addMassObject(const MassObject& massObj) {
    massObjects.push_back(massObj);
}

void MassObjectTracker::removeMassObject(size_t index) {
    if (index < massObjects.size()) {
        massObjects.erase(massObjects.begin() + index);
    }
}

const std::vector<MassObject>& MassObjectTracker::getMassObjects() const {
    return massObjects;
}

Sphere MassObjectTracker::getSphereForMassObject(size_t index) const {
    if (index >= massObjects.size()) {
        // Return default sphere if index is out of bounds
        return Sphere();
    }
    
    const MassObject& massObj = massObjects[index];
    
    // Convert 2D position to 3D (z = 0)
    glm::vec2 pos2D = massObj.getPosition();
    glm::vec3 pos3D(pos2D.x, 0.0f, pos2D.y);
    
    float radius = getRadiusFromMass(massObj.getMass());
    glm::vec4 color = getColorFromMass(massObj.getMass());
    
    return Sphere(pos3D, radius, color);
}

std::vector<Sphere> MassObjectTracker::getAllSpheres() const {
    std::vector<Sphere> spheres;
    spheres.reserve(massObjects.size());
    
    for (size_t i = 0; i < massObjects.size(); ++i) {
        spheres.push_back(getSphereForMassObject(i));
    }
    
    return spheres;
}

void MassObjectTracker::clear() {
    massObjects.clear();
}

size_t MassObjectTracker::getCount() const {
    return massObjects.size();
}

float MassObjectTracker::getRadiusFromMass(double mass) const {
    // Logarithmic scaling - perfect for massive range of solar masses
    // Formula: baseRadius + scaleFactor × (log10(mass) - minLog) / (maxLog - minLog)
    // 
    // This handles the huge range from Sun (1.0 M☉) to asteroids (10⁻¹⁰ M☉)
    // Sun: log10(1.0) = 0 → max radius
    // Jupiter: log10(0.001) = -3 → 70% of max
    // Earth: log10(3e-6) ≈ -5.5 → 45% of max  
    // Asteroid: log10(1e-10) = -10 → min radius
    //
    // To adjust:
    // - Change minLog/maxLog to adjust the mass range
    // - Increase scaleFactor (0.15f) to make size differences more pronounced
    // - Increase baseRadius (0.05f) to make smallest objects bigger
    // - Adjust max clamp (0.25f) to change largest object size
    
    // Use log10 scaling with offset
    double logMass = std::log10(std::max(mass, 1e-12)); // Prevent log(0)
    double minLog = -10.0; // Minimum mass range (10⁻¹⁰ M☉)
    double maxLog = 0.0;   // Maximum mass range (1.0 M☉)
    
    // Normalize to 0-1 range, then scale
    double normalizedLog = (logMass - minLog) / (maxLog - minLog);
    
    // Base radius + logarithmic scaling
    float radius = (0.05f + 0.15f * static_cast<float>(normalizedLog)) * 3.0f;
    //             ^base   ^scale factor
    
    // Clamp to reasonable visualization bounds  
    return std::max(0.25f, std::min(radius, 1.25f));
    //               ^min         ^max
}

void MassObjectTracker::updatePhysics(double deltaTime) {
    if (physicsEngine && !massObjects.empty()) {
        physicsEngine->update(massObjects, deltaTime);
    }
}

void MassObjectTracker::setPhysicsEnabled(bool enabled) {
    if (physicsEngine) {
        physicsEngine->setEnabled(enabled);
    }
}

void MassObjectTracker::switchToEulerIntegrator() {
    if (physicsEngine) {
        physicsEngine->setIntegrator(std::make_unique<EulerIntegrator>());
        std::cout << "Switched to Euler integration\n";
    }
}

void MassObjectTracker::switchToVerletIntegrator() {
    if (physicsEngine) {
        physicsEngine->setIntegrator(std::make_unique<VerletIntegrator>());
        std::cout << "Switched to Verlet integration\n";
    }
}

glm::vec4 MassObjectTracker::getColorFromMass(double mass) const {
    // Color coding based on solar mass ranges
    if (mass >= 0.1) {
        return glm::vec4(1.0f, 0.8f, 0.2f, 1.0f); // Yellow-orange for massive stars
    } else if (mass >= 0.0001) {
        return glm::vec4(0.2f, 0.4f, 1.0f, 1.0f); // Blue for gas giants
    } else if (mass >= 1e-6) {
        return glm::vec4(0.6f, 0.4f, 0.2f, 1.0f); // Brown for rocky planets
    } else {
        return glm::vec4(0.8f, 0.8f, 0.8f, 1.0f); // Gray for asteroids/small objects
    }
} 