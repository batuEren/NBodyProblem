#pragma once
#include <glm/glm.hpp>
class MassObject
{
	public:
        MassObject();  // Default constructor
        MassObject(double mass, glm::vec2 pos); // Overloaded constructor
        MassObject(double mass, glm::vec2 pos, glm::vec2 vel); // Constructor with initial velocity

        double getMass() const;
        glm::vec2 getPosition() const;
        glm::vec2 getVelocity() const;
        glm::vec2 getAcceleration() const;

        void setMass(double m);
        void setPosition(const glm::vec2& pos);
        void setVelocity(const glm::vec2& vel);
        void setAcceleration(const glm::vec2& accel);

        float distanceTo(const MassObject& other) const;

	private:
		double mass = 1.0; // mass in kilograms
        glm::vec2 position = glm::vec2(0.0, 0.0); // in kilometers
        glm::vec2 velocity = glm::vec2(0.0, 0.0); // in kilometers per second
        glm::vec2 acceleration = glm::vec2(0.0, 0.0); // in kilometers per second squared

};

