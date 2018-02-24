#pragma once

#include "Rigidbody.h"

class Rigidbody;

class Sphere : public Rigidbody
{
public:
	Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, float rotation, glm::vec4 colour, float linearDrag, float angularDrag, float elasticity = 1.0f);
	~Sphere();

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);

	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }
	float getRotation() { return m_rotation; }

	bool isInside(glm::vec2 pt);

protected:
	float m_radius;
	glm::vec4 m_colour;
};

