#include "Rigidbody.h"


#define MIN_LINEAR_THRESHOLD 0.1
#define MIN_ROTATIONAL_THRESHOLD 0.01

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float linearDrag, float angularDrag, float elasticity)
	: PhysicsObject(shapeID), m_position(position), m_velocity(velocity), m_rotation(rotation), m_mass(mass), m_elasticity(elasticity)
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	//m_velocity -= m_velocity * m_linearDrag * timeStep;

	//m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	//if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
	//	m_velocity = glm::vec2(0, 0);
	//if (glm::abs(m_angularVelocity) < MIN_ROTATIONAL_THRESHOLD)
	//	m_angularVelocity = 0;

	m_position += m_velocity * timeStep;

	applyForce(gravity * m_mass * timeStep);
}

void Rigidbody::debug()
{
}

void Rigidbody::applyForce(glm::vec2 force)
{
	m_velocity += force / m_mass;
}

void Rigidbody::applyForceToActor(Rigidbody * actor2, glm::vec2 force)
{
	applyForce(-force);
	actor2->applyForce(force);
}

void Rigidbody::resolveCollision(Rigidbody * actor2)
{
	glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position);
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;

	float j = glm::dot(-(1 + m_elasticity) * (relativeVelocity), normal) / glm::dot(normal, normal * ((1 / m_mass) + (1 / actor2->getMass())));

	glm::vec2 force = normal * j;

	if (glm::dot(normal, relativeVelocity) > 0)
		return;

	applyForceToActor(actor2, force);
}

void Rigidbody::setVelocity(glm::vec2 velocity)
{
	m_velocity = velocity;
}
