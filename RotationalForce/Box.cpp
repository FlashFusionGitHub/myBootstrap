#include "Box.h"
#include "PhysicsObject.h"
#include <Gizmos.h>
#include <vector>

Box::Box(glm::vec2 position, glm::vec2 velocity, float mass, float width, float height, float rotation, glm::vec4 colour, float linearDrag, float angularDrag, float elasticity)
	: Rigidbody(ShapeType::BOX, position, velocity, rotation, mass, linearDrag, angularDrag, elasticity)
{
	m_width = width;
	m_height = height;
	m_colour = colour;

	m_moment = 1.0f / 12.0f * mass * width * height;
}

Box::~Box()
{
}

void Box::makeGizmo()
{
	//aie::Gizmos::add2DAABBFilled(this->getPosition(), glm::vec2(this->getWidth(), this->getHeight()), this->getColour());

	glm::vec2 p1 = m_position - m_localX * (m_width / 2) - m_localY * (m_height / 2);
	glm::vec2 p2 = m_position + m_localX * (m_width / 2) - m_localY * (m_height / 2);
	glm::vec2 p3 = m_position - m_localX * (m_width / 2) + m_localY * (m_height / 2);
	glm::vec2 p4 = m_position + m_localX * (m_width / 2) + m_localY * (m_height / 2);

	aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
}

bool Box::checkCollision(PhysicsObject * pOther)
{
	Box* other = dynamic_cast<Box*>(pOther);

	if (this->getPosition().x - this->getWidth() < other->getPosition().x + other->getWidth()
		&& this->getPosition().x + this->getWidth() > other->getPosition().x - other->getWidth()
		&& this->getPosition().y - this->getHeight() < other->getPosition().y + other->getWidth()
		&& this->getPosition().y + this->getHeight() > other->getPosition().y - other->getWidth())
		return true;
	else
		return false;
}

bool Box::checkBoxCorners(const Box & box, glm::vec2 & contact, int & numContacts, glm::vec2 & edgeNormal, glm::vec2& contactForce)
{
	float boxW = (box.getWidth() / 2) * 2;
	float boxH = (box.getHeight() / 2) * 2;

	float penetration = 0;

	std::vector<glm::vec2> boxPoints;

	boxPoints.push_back(glm::vec2(-(box.getWidth()), -(box.getHeight())));
	boxPoints.push_back(glm::vec2(-(box.getWidth() / 2), 0));
	boxPoints.push_back(glm::vec2(-(box.getWidth()), box.getHeight()));
	boxPoints.push_back(glm::vec2(0, box.getHeight() / 2));
	boxPoints.push_back(glm::vec2(box.getWidth(), box.getHeight()));
	boxPoints.push_back(glm::vec2(box.getWidth() / 2, 0));
	boxPoints.push_back(glm::vec2(box.getWidth(), -(box.getHeight())));
	boxPoints.push_back(glm::vec2(0, -(box.getHeight() / 2)));

		// pos in worldspace
	for (auto point : boxPoints) {
		glm::vec2 p = box.m_position + point.x * box.m_localX + point.y * box.m_localY;
		// position in our box's space
		glm::vec2 p0(glm::dot(p - m_position, m_localX), glm::dot(p - m_position, m_localY));
		float w2 = (m_width / 2), h2 = (m_height / 2);
		if (p0.y < h2 && p0.y > -h2) {
			if (p0.x > 0 && p0.x < w2) {
				numContacts++;
				contact += m_position + w2 * m_localX + p0.y * m_localY;
				edgeNormal = m_localX;
				penetration = w2 - p0.x;
			}
			if (p0.x < 0 && p0.x > -w2) {
				numContacts++;
				contact += m_position - w2 * m_localX + p0.y * m_localY;
				edgeNormal = -m_localX;
				penetration = w2 + p0.x;
			}
		}
		if (p0.x < w2 && p0.x > -w2) {
			if (p0.y > 0 && p0.y < h2) {
				numContacts++;
				contact += m_position + p0.x * m_localX + h2 * m_localY;
				float pen0 = h2 - p0.y;
				if (pen0 < penetration || penetration == 0) {
					penetration = pen0;
					edgeNormal = m_localY;
				}
			}
			if (p0.y < 0 && p0.y > -h2) {
				numContacts++;
				contact += m_position + p0.x * m_localX - h2 * m_localY;
				float pen0 = h2 + p0.y;
				if (pen0 < penetration || penetration == 0) {
					penetration = pen0;
					edgeNormal = -m_localY;
				}
			}
		}
	}
	contactForce = penetration * edgeNormal;
	return (penetration != 0);
}

void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	Rigidbody::fixedUpdate(gravity, timeStep);

	//store the local axes
	float cs = std::cosf(m_rotation);
	float sn = std::sinf(m_rotation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}
