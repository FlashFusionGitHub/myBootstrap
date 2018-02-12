#pragma once

#include "Rigidbody.h"

class Rigidbody;

class AABB : public Rigidbody
{
public:
	AABB(glm::vec2 position, glm::vec2 velocity, float mass, float width, float height, float rotation, glm::vec4 colour);
	~AABB();

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);

	float getWidth() { return m_width; }
	float getHeight() { return m_height; }
	float getRotation() { return m_rotation; }
	glm::vec4 getColour() { return m_colour; }

protected:
	float m_width;
	float m_height;
	float m_rotation;
	glm::vec4 m_colour;
};

