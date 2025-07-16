#pragma once
#include <glm/glm.hpp>
class MassObject
{
	public:
        MassObject();  // Default constructor
        MassObject(double mass); // Overloaded constructor

        double getMass() const;

        void setMass(double m);



	private:
		float mass = 1.0f; // mass in kilograms

};

