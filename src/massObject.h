#pragma once
#include <glm/glm.hpp>
class MassObject
{
	public:
        MassObject();  // Default constructor
        MassObject(double mass, glm::vec2 pos); // Overloaded constructor

        double getMass() const;
        glm::vec2 getPosition() const;

        void setMass(double m);
        void setPosition(const glm::vec2& pos);

        float distanceTo(const MassObject& other) const;

	private:
		float mass = 1.0f; // mass in kilograms
        glm::vec2 position = glm::vec2(0.0f, 0.0f); // in kilometers

};

