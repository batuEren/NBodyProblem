#ifndef GRID_GENERATOR_H
#define GRID_GENERATOR_H

#include <vector>

// Generates a grid of lines on the XZ plane centered at origin
// @param size: number of grid lines from center to each side
// @param spacing: distance between adjacent lines
// @return flat array of vertex positions (x, y, z) for GL_LINES
std::vector<float> generateGridVertices(int size, float spacing = 0.25f);

#endif // GRID_GENERATOR_H