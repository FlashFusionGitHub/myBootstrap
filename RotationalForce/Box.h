#pragma once

#include "Rigidbody.h"

class Rigidbody;

class Box : public Rigidbody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float mass, float width, float height, float rotation, glm::vec4 colour, float linearDrag, float angularDrag, float elasticity = 1);
	~Box();

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);

	bool checkBoxCorners(const Box& box, glm::vec2& contact, int& numContacts, float &pen, glm::vec2& edgeNormal);
	float getWidth() { return m_width; }
	float getHeight() { return m_height; }
	float getRotation() { return m_rotation; }
	void fixedUpdate(glm::vec2 gravity, float timeStep);

    glm::vec4 getColour() { return m_colour; }
   
    glm::vec2 getLocalX() { return m_localX; }
    glm::vec2 getLocalY() { return m_localY; }

protected:
	float m_width;
	float m_height;

	glm::vec4 m_colour;

	glm::vec2 m_localX;
	glm::vec2 m_localY;
};

