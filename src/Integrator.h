#pragma once
#include "MassObject.h"
#include <vector>
#include <glm/glm.hpp>

// Abstract base class for numerical integration methods
class Integrator {
public:
    virtual ~Integrator() = default;
    
    // Integrate one timestep given current state and forces
    virtual void integrate(std::vector<MassObject>& massObjects, 
                          const std::vector<glm::vec2>& forces, 
                          double deltaTime) const = 0;
};

// Simple Euler integration (first-order, less stable but simple)
class EulerIntegrator : public Integrator {
public:
    void integrate(std::vector<MassObject>& massObjects, 
                   const std::vector<glm::vec2>& forces, 
                   double deltaTime) const override;
};

// Velocity Verlet integration (second-order, more stable)
class VerletIntegrator : public Integrator {
public:
    void integrate(std::vector<MassObject>& massObjects, 
                   const std::vector<glm::vec2>& forces, 
                   double deltaTime) const override;
    
private:
    // Store previous accelerations for Verlet method
    mutable std::vector<glm::vec2> previousAccelerations;
}; 