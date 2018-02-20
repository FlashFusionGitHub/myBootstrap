#include "Sphere.h"
#include <glm\glm.hpp>
#include <Gizmos.h>
#include <iostream>


Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, float rotation, glm::vec4 colour, float linearDrag, float angularDrag, float elasticity) 
	: Rigidbody(SPHERE, position, velocity, rotation, mass, linearDrag, angularDrag, elasticity)
{
	m_radius = radius;
	m_rotation = rotation;
	m_moment = 0.5f * mass * radius * radius;
	m_colour = colour;
}

Sphere::~Sphere()
{
}

void Sphere::makeGizmo()
{
	glm::vec2 end = glm::vec2(std::cos(m_rotation), std::sin(m_rotation)) * m_radius;

	aie::Gizmos::add2DCircle(this->getPosition(), this->getRadius(), 32, this->getColour());
	aie::Gizmos::add2DLine(this->getPosition(), this->getPosition() + end, glm::vec4(1, 1, 1, 1));
}

bool Sphere::checkCollision(PhysicsObject * pOther)
{
	Sphere* other = dynamic_cast<Sphere*>(pOther);

	if (glm::distance(this->getPosition(), other->getPosition()) < this->getRadius() + other->getRadius())
		return true;
	else
		return false;
}
