#pragma once
#include "ForceCalculator.h"
class BarnesHutCalculator : public ForceCalculator
{
public:
    BarnesHutCalculator(double gravitationalConstant = 39.478, double theta = 0.5, double softening = 0.001);

    std::vector<glm::vec2> calculateForces(const std::vector<MassObject>& massObjects) const override;

    void setTheta(double t) { theta = t; }
    double getTheta() const { return theta; }

    void setSoftening(double s) { softening = s; }
    double getSoftening() const { return softening; }

private:
    double G;
    double theta;
    double softening;
};

