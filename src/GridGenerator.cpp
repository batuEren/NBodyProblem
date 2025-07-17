#include "GridGenerator.h"

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
