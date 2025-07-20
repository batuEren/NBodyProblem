#pragma once
#include "MassObject.h"
#include "Sphere.h"
#include <vector>

class MassObjectTracker {
public:
    MassObjectTracker();
    
    // Add a mass object to the tracker
    void addMassObject(const MassObject& massObj);
    
    // Remove mass object by index
    void removeMassObject(size_t index);
    
    // Get all mass objects
    const std::vector<MassObject>& getMassObjects() const;
    
    // Get sphere representation for a mass object
    Sphere getSphereForMassObject(size_t index) const;
    
    // Get all spheres for rendering
    std::vector<Sphere> getAllSpheres() const;
    
    // Clear all mass objects
    void clear();
    
    // Get count of mass objects
    size_t getCount() const;

private:
    std::vector<MassObject> massObjects;
    
    // Helper function to determine sphere radius based on mass
    float getRadiusFromMass(double mass) const;
    
    // Helper function to determine sphere color based on mass
    glm::vec4 getColorFromMass(double mass) const;
}; 