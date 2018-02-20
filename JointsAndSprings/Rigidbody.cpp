#include "Rigidbody.h"


#define MIN_LINEAR_THRESHOLD 0.0
#define MIN_ROTATIONAL_THRESHOLD 0.01

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float linearDrag, float angularDrag, float elasticity)
	: PhysicsObject(shapeID), m_position(position), m_velocity(velocity), m_rotation(rotation), m_mass(mass), m_linearDrag(linearDrag), m_angularDrag(angularDrag), m_elasticity(elasticity)
{
	m_angularVelocity = 0;
	m_isKinematic = false;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{

	if (m_isKinematic)
		return;

	m_position += m_velocity * timeStep;

	m_velocity += gravity * timeStep;

	m_velocity -= m_velocity * m_linearDrag * timeStep;

	m_rotation += m_angularVelocity * timeStep;

	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
		m_velocity = glm::vec2(0, 0);
	if (glm::abs(m_angularVelocity) < MIN_ROTATIONAL_THRESHOLD)
		m_angularVelocity = 0;
}

void Rigidbody::debug()
{
}

void Rigidbody::applyForce(glm::vec2 force, glm::vec2 position)
{
	m_velocity += force / m_mass;
    m_angularVelocity += (force.y * position.x - force.x * position.y) / (m_moment);
}

void Rigidbody::resolveCollision(Rigidbody * actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
	glm::vec2 normal = collisionNormal ? *collisionNormal : glm::normalize(actor2->m_position - m_position);

	glm::vec2 perp(normal.y, -normal.x);

	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->m_position, perp);
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

	if (v1 > v2) {
		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2*r2) / actor2->m_moment);

		float elasticity = (m_elasticity = actor2->getElasticity()) / 2.0f;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

		applyForce(-force, contact - m_position);
		actor2->applyForce(force, contact - actor2->m_position);
	}
}

void Rigidbody::setVelocity(glm::vec2 velocity)
{
	m_velocity = velocity;
}

void Rigidbody::setPosition(glm::vec2 position)
{
	m_position = position;
}
