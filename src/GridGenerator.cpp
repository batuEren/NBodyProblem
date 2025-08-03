#include "GridGenerator.h"
#include "MassObject.h"
#include "ForceCalculator.h"

GridGenerator::GridGenerator() {
}

std::vector<float> GridGenerator::generateGridVertices(int size, float spacing) {
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

std::vector<float> GridGenerator::generateGridVertices(int size, float spacing, const std::vector<MassObject>& massObjects) { // only creates the edge verticies rn must also have intersections
    std::vector<float> vertices;

    // Initialize lastMatrix if it's empty or wrong size
    int matrixSize = (2 * size + 1) * (2 * size + 1);
    if (lastMatrix.size() != matrixSize) {
        lastMatrix.resize(matrixSize, 0.0f);
    }

    // Create calculator once outside the loop for efficiency
    BruteForceCalculator calc(39.478); // Use the same G value as the physics engine

    // Scaling factor to make field visualization visible
    float visualizationScale = 1000.0f;

    for (int x = -size; x <= size; ++x) {
        for (int z = -size; z <= size; ++z) {
            float yCoord = glm::length(calc.calculateGravitationalField(glm::vec2(x * spacing, z * spacing), massObjects)) * visualizationScale;
            //yCoord = -glm::log2(yCoord)/glm::log2(3.0f);
            //yCoord = -(1 - 1/glm::sqrt(yCoord));
            //yCoord = -yCoord / (yCoord + 3);
            float alpha = 33.5f;
            yCoord = -glm::log(1 + yCoord * alpha) / glm::log(1 + alpha);

            float changeCoef = 0.1f;

            lastMatrix[(x + size) * (2 * size + 1) + (z + size)] = lastMatrix[(x + size) * (2 * size + 1) + (z + size)] * (1.0f - changeCoef)
                + yCoord * changeCoef;

            //matrix.push_back(yCoord); // save each yCoord calculated in a matrix
        }
    }
    for (int x = -size; x <= size; ++x) {// now put ycoord vals into a vector that is suitable to be drawn by GL_LINES
        for (int z = -size; z <= size; ++z) {
            if (x != -size) { //convert from 2d x,z coord to 1d coord index
                vertices.push_back(x * spacing); vertices.push_back(lastMatrix[(x + size) * (2 * size + 1) + (z + size)]); vertices.push_back(z * spacing);
                vertices.push_back((x - 1) * spacing); vertices.push_back(lastMatrix[((x + size) - 1) * (2 * size + 1) + (z + size)]); vertices.push_back(z * spacing);
            }
            //if (x != size) { 
            //    vertices.push_back(x * spacing); vertices.push_back(matrix[(x + size) * 2 * size + (z + size)]); vertices.push_back(z * spacing);
            //    vertices.push_back((x + 1)* spacing); vertices.push_back(matrix[((x + size) + 1) * 2 * size + (z + size)]); vertices.push_back(z * spacing);
            //}
            if (z != -size) { //convert from 2d x,z coord to 1d coord index
                vertices.push_back(x * spacing); vertices.push_back(lastMatrix[(x + size) * (2 * size + 1) + (z + size)]); vertices.push_back(z * spacing);
                vertices.push_back(x * spacing); vertices.push_back(lastMatrix[(x + size) * (2 * size + 1) + ((z + size) - 1)]); vertices.push_back((z - 1) * spacing);
            }
            //if (z != size) { //convert from 2d x,z coord to 1d coord index
            //    vertices.push_back(x * spacing); vertices.push_back(matrix[(x + size) * (2*size+1) + (z + size)]); vertices.push_back(z * spacing);
            //    vertices.push_back(x * spacing); vertices.push_back(matrix[(x + size) * (2*size+1) + ((z + size) + 1)]); vertices.push_back((z + 1) * spacing);
            //}
        }
    }

    /*
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
    }*/

    return vertices;
}
