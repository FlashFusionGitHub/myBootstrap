#pragma once

#include <glm\glm.hpp>
#include <list>
#include "PhysicsObject.h"

class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void addActor(PhysicsObject* actors);
	void removeActor(PhysicsObject* actors);
	void update(float deltaTime);
	void updateGizmos();
	void debugScene();

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

	void checkForCollision();

	static bool plane2Plane(PhysicsObject*, PhysicsObject*);
	static bool plane2Sphere(PhysicsObject*, PhysicsObject*);
	static bool plane2AABB(PhysicsObject*, PhysicsObject*);
	static bool sphere2Sphere(PhysicsObject*, PhysicsObject*);
	static bool sphere2Plane(PhysicsObject*, PhysicsObject*);
	static bool sphere2AABB(PhysicsObject*, PhysicsObject*);
	static bool aabb2AABB(PhysicsObject*, PhysicsObject*);
	static bool aabb2Plane(PhysicsObject*, PhysicsObject*);
	static bool aabb2Sphere(PhysicsObject*, PhysicsObject*);

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::list<PhysicsObject*> m_actors;
};

