#include "Sphere.h"
#include <glm\glm.hpp>
#include <Gizmos.h>
#include <iostream>


Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) : Rigidbody(SPHERE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_colour = colour;
}

Sphere::~Sphere()
{
}

void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(this->getPosition(), this->getRadius(), 32, this->getColour());
}

bool Sphere::checkCollision(PhysicsObject * pOther)
{
	Sphere* other = dynamic_cast<Sphere*>(pOther);

	if (glm::distance(this->getPosition(), other->getPosition()) < this->getRadius() + other->getRadius())
		return true;
	else
		return false;
}
