#include "Spring.h"
#include <Gizmos.h>

Spring::Spring(Rigidbody * body1, Rigidbody * body2, float restLength, float springCoefficent, float damping = 0.1f, glm::vec2 contact1, glm::vec2 contact2)
	: PhysicsObject(ShapeType::JOINT)
{
	m_body1 = body1;
	m_body2 = body2;
	m_restLength = restLength;
	m_springCoefficient = springCoefficent;
	m_damping = damping;
	m_contact1 = contact1;
	m_contact2 = contact2;
}

Spring::~Spring()
{
}

void Spring::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	glm::vec2 p2 = m_body2->getPosition();
	glm::vec2 p1 = m_body1->getPosition();

	glm::vec2 distance = p2 - p1;

	float length = sqrtf(distance.x * distance.x + distance.y * distance.y);

	//apply damping
	glm::vec2 relativeVelocity = m_body2->getVelocity() - m_body1->getVelocity();

	//F = -kX - bv
	glm::vec2 force = distance * m_springCoefficient * (m_restLength - length) - m_damping * relativeVelocity;

	m_body1->applyForce(-force * timeStep, p1 - m_body1->getPosition());
	m_body2->applyForce(force * timeStep, p1 - m_body1->getPosition());
}

void Spring::makeGizmo()
{
	aie::Gizmos::add2DLine(m_body2->getPosition(), m_body1->getPosition(), glm::vec4(1, 1, 1, 1));
}
