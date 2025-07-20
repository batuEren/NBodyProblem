#include "MassObject.h"
#include <glm/glm.hpp>

MassObject::MassObject() = default;

MassObject::MassObject(double mass, glm::vec2 pos)
    : mass(mass), position(pos) {}

MassObject::MassObject(double mass, glm::vec2 pos, glm::vec2 vel)
    : mass(mass), position(pos), velocity(vel) {}

double MassObject::getMass() const {
    return mass;
}

glm::vec2 MassObject::getPosition() const {
    return position;
}

glm::vec2 MassObject::getVelocity() const {
    return velocity;
}

glm::vec2 MassObject::getAcceleration() const {
    return acceleration;
}

void MassObject::setMass(double m) {
    mass = m;
}

void MassObject::setPosition(const glm::vec2& pos) {
    position = pos;
}

void MassObject::setVelocity(const glm::vec2& vel) {
    velocity = vel;
}

void MassObject::setAcceleration(const glm::vec2& accel) {
    acceleration = accel;
}

float MassObject::distanceTo(const MassObject& other) const {
    return glm::distance(position, other.position);
}
