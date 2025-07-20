#include "Integrator.h"


// integration using the euler method for gravity calculations
void EulerIntegrator::integrate(std::vector<MassObject>& massObjects, 
                               const std::vector<glm::vec2>& forces, 
                               double deltaTime) const {
    
    for (size_t i = 0; i < massObjects.size(); ++i) {
        MassObject& obj = massObjects[i];
        
        // Calculate acceleration: a = F / m
        glm::vec2 acceleration = forces[i] / static_cast<float>(obj.getMass());
        
        // Update velocity: v = v + a * dt
        glm::vec2 newVelocity = obj.getVelocity() + acceleration * static_cast<float>(deltaTime);
        
        // Update position: x = x + v * dt
        glm::vec2 newPosition = obj.getPosition() + newVelocity * static_cast<float>(deltaTime);
        
        // Apply updates
        obj.setAcceleration(acceleration);
        obj.setVelocity(newVelocity);
        obj.setPosition(newPosition);
    }
}

void VerletIntegrator::integrate(std::vector<MassObject>& massObjects, 
                                const std::vector<glm::vec2>& forces, 
                                double deltaTime) const {
    
    // Resize previous accelerations if needed
    if (previousAccelerations.size() != massObjects.size()) {
        previousAccelerations.resize(massObjects.size(), glm::vec2(0.0, 0.0));
    }
    
    for (size_t i = 0; i < massObjects.size(); ++i) {
        MassObject& obj = massObjects[i];
        
        // Calculate current acceleration: a = F / m
        glm::vec2 currentAcceleration = forces[i] / static_cast<float>(obj.getMass());
        
        // Velocity Verlet integration
        // v(t + dt) = v(t) + (a(t) + a(t + dt)) * dt / 2
        // x(t + dt) = x(t) + v(t) * dt + a(t) * dt² / 2
        
        glm::vec2 currentVelocity = obj.getVelocity();
        glm::vec2 currentPosition = obj.getPosition();
        glm::vec2 previousAcceleration = previousAccelerations[i];
        
        float dt = static_cast<float>(deltaTime);
        
        // Update position first
        glm::vec2 newPosition = currentPosition + currentVelocity * dt + 
                               previousAcceleration * (dt * dt * 0.5f);
        
        // Update velocity using average of previous and current acceleration
        glm::vec2 newVelocity = currentVelocity + 
                               (previousAcceleration + currentAcceleration) * (dt * 0.5f);
        
        // Apply updates
        obj.setPosition(newPosition);
        obj.setVelocity(newVelocity);
        obj.setAcceleration(currentAcceleration);
        
        // Store current acceleration for next timestep
        previousAccelerations[i] = currentAcceleration;
    }
} 