#include "PhysicsEngine.h"
#include "ForceCalculator.h"
#include "Integrator.h"
#include <algorithm>

PhysicsEngine::PhysicsEngine()
    : physicsTimestep(0.001), // 1ms default timestep for stability
      physicsEnabled(true),
      accumulatedTime(0.0)
{
    // Default to brute force calculator and Euler integrator
    forceCalculator = std::make_unique<BruteForceCalculator>();
    integrator = std::make_unique<EulerIntegrator>();
}

PhysicsEngine::~PhysicsEngine() = default;

void PhysicsEngine::setForceCalculator(std::unique_ptr<ForceCalculator> calculator) {
    forceCalculator = std::move(calculator);
}

void PhysicsEngine::setIntegrator(std::unique_ptr<Integrator> integrator) {
    this->integrator = std::move(integrator);
}

void PhysicsEngine::update(std::vector<MassObject>& massObjects, double deltaTime) {
    if (!physicsEnabled || !forceCalculator || !integrator) {
        return;
    }
    
    // Accumulate time and process in small timesteps for stability
    accumulatedTime += deltaTime;
    
    while (accumulatedTime >= physicsTimestep) {
        // Calculate forces for all objects
        std::vector<glm::vec2> forces = forceCalculator->calculateForces(massObjects);
        
        // Integrate one timestep
        integrator->integrate(massObjects, forces, physicsTimestep);
        
        accumulatedTime -= physicsTimestep;
    }
}

void PhysicsEngine::setPhysicsTimestep(double timestep) {
    physicsTimestep = std::max(0.0001, timestep); // Minimum 0.1ms timestep for stability
}

double PhysicsEngine::calculateTotalEnergy(const std::vector<MassObject>& massObjects) const {
    if (!forceCalculator) return 0.0;
    
    double kineticEnergy = 0.0;
    double potentialEnergy = 0.0;
    
    // Calculate kinetic energy: KE = 0.5 * m * v²
    for (const auto& obj : massObjects) {
        glm::vec2 velocity = obj.getVelocity();
        double speed2 = velocity.x * velocity.x + velocity.y * velocity.y;
        kineticEnergy += 0.5 * obj.getMass() * speed2;
    }
    
    // Calculate gravitational potential energy: PE = -G * m1 * m2 / r
    for (size_t i = 0; i < massObjects.size(); ++i) {
        for (size_t j = i + 1; j < massObjects.size(); ++j) {
            double distance = massObjects[i].distanceTo(massObjects[j]);
            if (distance > 0.01) { // Avoid division by zero
                potentialEnergy -= (1.0 * massObjects[i].getMass() * massObjects[j].getMass()) / distance;
            }
        }
    }
    
    return kineticEnergy + potentialEnergy;
} 