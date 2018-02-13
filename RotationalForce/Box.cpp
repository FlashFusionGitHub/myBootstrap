#include "Box.h"
#include "PhysicsObject.h"
#include <Gizmos.h>

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

bool Box::checkBoxCorners(const Box & box, glm::vec2 & contact, int & numContacts, float & pen, glm::vec2 & edgeNormal)
{
	float minX, maxX, minY, maxY;
	float boxW = (box.m_width / 2) * 2;
	float boxH = (box.m_height / 2) * 2;

	int numLocalContacts = 0;
	glm::vec2 localContact(0, 0);

	bool first = true;

	for (float x = (-box.m_width / 2); x < boxW; x += boxW) {
		for (float y = (-box.m_height / 2); y < boxH; y += boxH) {
			glm::vec2 p = box.m_position + x * box.m_localX + y * box.m_localY;
			glm::vec2 p0(glm::dot(p - m_position, m_localX), glm::dot(p - m_position, m_localY));

			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > maxX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;

			if (p0.x >= -(m_width / 2) && p0.x <= (m_width / 2) && p0.y >= -(m_height/ 2) && p0.y <= (m_height / 2)) {
				numLocalContacts++;
				localContact += p0;	
			}		
			first = false;
		}
	}

	if (maxX <-(m_width / 2)|| minX > (m_width / 2) || maxY < -(m_height / 2) || minY > (m_height / 2))
		return false;
	if (numLocalContacts == 0)
		return false;

	bool res = false;

	contact += m_position + (localContact.x*m_localX + localContact.y*m_localY) / (float)numLocalContacts;

	numContacts++;

	float pen0 = (m_width / 2) - minX;

	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = m_localX; pen = pen0; res = true; 
	} 
	
	pen0 = maxX + (m_width / 2);

	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = -m_localX; pen = pen0; res = true; 
	} 
	
	pen0 = (m_height / 2) - minY;
	
	if (pen0 > 0 && (pen0 < pen || pen == 0)) { 
		edgeNormal = m_localY; pen = pen0; res = true; 
	} 
	
	pen0 = maxY + (m_height / 2);

	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = -m_localY; pen = pen0; res = true; 
	} 
	
	return res;
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
