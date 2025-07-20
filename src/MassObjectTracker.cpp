#include "MassObjectTracker.h"
#include <glm/glm.hpp>
#include <cmath>

MassObjectTracker::MassObjectTracker() {}

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
    glm::vec3 pos3D(pos2D.x, pos2D.y, 0.0f);
    
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
    // Scale radius based on mass 
    // Using cube root to make the volume proportional to mass
    return static_cast<float>(0.5 + 0.3 * std::cbrt(mass));
}

glm::vec4 MassObjectTracker::getColorFromMass(double mass) const {
    // Color coding based on mass (you can customize this)
    if (mass < 2.0) {
        return glm::vec4(0.2f, 0.2f, 1.0f, 1.0f); // Blue for small masses
    } else if (mass < 5.0) {
        return glm::vec4(0.2f, 1.0f, 0.2f, 1.0f); // Green for medium masses
    } else if (mass < 10.0) {
        return glm::vec4(1.0f, 1.0f, 0.2f, 1.0f); // Yellow for large masses
    } else {
        return glm::vec4(1.0f, 0.2f, 0.2f, 1.0f); // Red for very large masses
    }
} 