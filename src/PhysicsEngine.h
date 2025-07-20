#pragma once
#include "MassObject.h"
#include <vector>
#include <memory>

// Forward declarations
class ForceCalculator;
class Integrator;

class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine();
    
    // Set the force calculation method (brute force, Barnes-Hut, etc.)
    void setForceCalculator(std::unique_ptr<ForceCalculator> calculator);
    
    // Set the numerical integration method
    void setIntegrator(std::unique_ptr<Integrator> integrator);
    
    // Advance simulation by given time step
    void update(std::vector<MassObject>& massObjects, double deltaTime);
    
    // Set physics timestep (internal subdivision of deltaTime)
    void setPhysicsTimestep(double timestep);
    
    // Get current physics timestep
    double getPhysicsTimestep() const { return physicsTimestep; }
    
    // Enable/disable physics simulation
    void setEnabled(bool enabled) { physicsEnabled = enabled; }
    bool isEnabled() const { return physicsEnabled; }
    
    // Debug/analysis methods
    double calculateTotalEnergy(const std::vector<MassObject>& massObjects) const;

private:
    std::unique_ptr<ForceCalculator> forceCalculator;
    std::unique_ptr<Integrator> integrator;
    double physicsTimestep;  // Internal physics timestep (smaller than render timestep)
    bool physicsEnabled;
    
    // Accumulate small timesteps to handle larger deltaTime
    double accumulatedTime;
}; 