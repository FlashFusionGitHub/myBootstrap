#include "AABB.h"
#include "PhysicsObject.h"
#include <Gizmos.h>

AABB::AABB(glm::vec2 position, glm::vec2 velocity, float mass, float width, float height, float rotation, glm::vec4 colour) : Rigidbody(ShapeType::BOX, position, velocity, rotation, mass)
{
	m_width = width;
	m_height = height;
	m_colour = colour;
}

AABB::~AABB()
{
}

void AABB::makeGizmo()
{
	aie::Gizmos::add2DAABBFilled(this->getPosition(), glm::vec2(this->getWidth(), this->getHeight()), this->getColour());
}

bool AABB::checkCollision(PhysicsObject * pOther)
{
	AABB* other = dynamic_cast<AABB*>(pOther);

	if (this->getPosition().x - this->getWidth() / 2 < other->getPosition().x + other->getWidth() / 2
		&& this->getPosition().x + this->getWidth() / 2 > other->getPosition().x - other->getWidth() / 2
		&& this->getPosition().y - this->getHeight() / 2 < other->getPosition().y + other->getWidth() / 2
		&& this->getPosition().y + this->getHeight() / 2 > other->getPosition().y - other->getWidth() / 2)
		return true;
	else
		return false;
}
