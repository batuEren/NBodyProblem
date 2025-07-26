#include "GridGenerator.h"
#include "MassObject.h"
#include "ForceCalculator.h"


std::vector<float> generateGridVertices(int size, float spacing) {
    std::vector<float> vertices;

    for (int i = -size; i <= size; ++i) {
        // Lines parallel to Z-axis (X = constant)
        vertices.push_back(i * spacing); vertices.push_back(0.0f); vertices.push_back(-size * spacing);
        vertices.push_back(i * spacing); vertices.push_back(0.0f); vertices.push_back(size * spacing);

        // Lines parallel to X-axis (Z = constant)
        vertices.push_back(-size * spacing); vertices.push_back(0.0f); vertices.push_back(i * spacing);
        vertices.push_back(size * spacing);  vertices.push_back(0.0f); vertices.push_back(i * spacing);
    }

    return vertices;
}

std::vector<float> generateGridVertices(int size, float spacing, const std::vector<MassObject>& massObjects) { // only creates the edge verticies rn must also have intersections
    std::vector<float> vertices;
    
    // Create calculator once outside the loop for efficiency
    BruteForceCalculator calc(39.478); // Use the same G value as the physics engine

    // Scaling factor to make field visualization visible
    float visualizationScale = 2000.0f;

    for (int i = -size; i <= size; ++i) {
        // Lines parallel to Z-axis (X = constant)
        float yCoord1 = -glm::length(calc.calculateGravitationalField(glm::vec2(i * spacing, -size * spacing), massObjects)) * visualizationScale;
        vertices.push_back(i * spacing); vertices.push_back(yCoord1); vertices.push_back(-size * spacing);

        float yCoord2 = -glm::length(calc.calculateGravitationalField(glm::vec2(i * spacing, size * spacing), massObjects)) * visualizationScale;
        vertices.push_back(i * spacing); vertices.push_back(yCoord2); vertices.push_back(size * spacing);

        // Lines parallel to X-axis (Z = constant)
        float yCoord3 = -glm::length(calc.calculateGravitationalField(glm::vec2(-size * spacing, i * spacing), massObjects)) * visualizationScale;
        vertices.push_back(-size * spacing); vertices.push_back(yCoord3); vertices.push_back(i * spacing);

        float yCoord4 = -glm::length(calc.calculateGravitationalField(glm::vec2(size * spacing, i * spacing), massObjects)) * visualizationScale;
        vertices.push_back(size * spacing); vertices.push_back(yCoord4); vertices.push_back(i * spacing);
    }

    return vertices;
}