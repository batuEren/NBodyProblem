#include "MassObject.h"
#include <glm/glm.hpp>

MassObject::MassObject() = default;

MassObject::MassObject(double mass, glm::vec2 pos)
    : mass(mass), position(pos) {}

double MassObject::getMass() const {
    return mass;
}

glm::vec2 MassObject::getPosition() const {
    return position;
}

void MassObject::setMass(double m) {
    mass = m;
}

void MassObject::setPosition(const glm::vec2& pos) {
    position = pos;
}


float MassObject::distanceTo(const MassObject& other) const {
    return glm::distance(position, other.position);
}
