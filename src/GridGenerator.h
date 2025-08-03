#ifndef GRID_GENERATOR_H
#define GRID_GENERATOR_H

#include <vector>
#include "MassObject.h"
class GridGenerator
{
public:
	GridGenerator();
	std::vector<float> lastMatrix;
	// Generates a grid of lines on the XZ plane centered at origin
	// @param size: number of grid lines from center to each side
	// @param spacing: distance between adjacent lines
	// @return flat array of vertex positions (x, y, z) for GL_LINES
	std::vector<float> generateGridVertices(int size, float spacing = 0.25f);

	// Generates a grid with gravitational field visualization
	// @param size: number of grid lines from center to each side  
	// @param spacing: distance between adjacent lines
	// @param massObjects: mass objects that create the gravitational field
	// @return flat array of vertex positions (x, y, z) for GL_LINES with field deformation
	std::vector<float> generateGridVertices(int size, float spacing, const std::vector<MassObject>& massObjects);

#endif // GRID_GENERATOR_H
};