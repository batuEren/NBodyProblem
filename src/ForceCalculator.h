#pragma once
#include "MassObject.h"
#include <vector>
#include <glm/glm.hpp>

// Abstract base class for different force calculation methods
class ForceCalculator {
public:
    virtual ~ForceCalculator() = default;
    
    // Calculate forces for all mass objects
    // Returns vector of net forces (same size and order as input)
    virtual std::vector<glm::vec2> calculateForces(const std::vector<MassObject>& massObjects) const = 0;
    
protected:
    // Gravitational constant in astronomical units
    // G = 4π² AU³/(M☉·year²) ≈ 39.48 AU³/(M☉·year²)
    // For simulation, we use a scaled version for better visual timing
    static constexpr double G_REAL = 39.478; // Real G in AU³/(M☉·year²)
    static constexpr double G_SIMULATION = 1.0; // Scaled for simulation timing
};

// Brute force O(n²) implementation
class BruteForceCalculator : public ForceCalculator {
public:
    BruteForceCalculator(double gravitationalConstant = 1.0);
    
    std::vector<glm::vec2> calculateForces(const std::vector<MassObject>& massObjects) const override;
    
    // Set minimum distance to avoid singularities
    void setSofteningParameter(double softening) { softeningParameter = softening; }
    double getSofteningParameter() const { return softeningParameter; }

private:
    double customG; // Allow custom gravitational constant
    double softeningParameter; // Prevent division by zero when objects are very close
    
    // Calculate gravitational force between two mass objects
    glm::vec2 calculateGravitationalForce(const MassObject& obj1, const MassObject& obj2) const;
}; 