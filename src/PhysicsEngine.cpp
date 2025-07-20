#include "PhysicsEngine.h"
#include "ForceCalculator.h"
#include "Integrator.h"
#include <algorithm>

PhysicsEngine::PhysicsEngine()
    : physicsTimestep(0.0001), // Smaller timestep needed for real G values (0.0001 years ≈ 0.88 hours)
      physicsEnabled(true),
      accumulatedTime(0.0)
{
    // Using real astronomical gravitational constant G = 4π² ≈ 39.478 AU³/(M☉·year²)
    forceCalculator = std::make_unique<BruteForceCalculator>(39.478); // Real astronomical G
    integrator = std::make_unique<VerletIntegrator>();
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
    physicsTimestep = std::max(0.00001, timestep); // Minimum 0.00001 years ≈ 5.25 minutes for real G stability
}

double PhysicsEngine::calculateTotalEnergy(const std::vector<MassObject>& massObjects) const {
    if (!forceCalculator) return 0.0;
    
    double kineticEnergy = 0.0;
    double potentialEnergy = 0.0;
    
    // Calculate kinetic energy: KE = 0.5 * m * v² (in AU²·M☉/year²)
    for (const auto& obj : massObjects) {
        glm::vec2 velocity = obj.getVelocity();
        double speed2 = velocity.x * velocity.x + velocity.y * velocity.y;
        kineticEnergy += 0.5 * obj.getMass() * speed2;
    }
    
    // Calculate gravitational potential energy: PE = -G * m1 * m2 / r (in AU·M☉/year²)
    for (size_t i = 0; i < massObjects.size(); ++i) {
        for (size_t j = i + 1; j < massObjects.size(); ++j) {
            double distance = massObjects[i].distanceTo(massObjects[j]);
            if (distance > 0.001) { // Avoid division by zero (0.001 AU ≈ 150,000 km)
                potentialEnergy -= (1.0 * massObjects[i].getMass() * massObjects[j].getMass()) / distance;
            }
        }
    }
    
    return kineticEnergy + potentialEnergy;
} 