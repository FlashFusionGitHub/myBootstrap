#pragma once

#include <glm\glm.hpp>
#include <list>

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

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

	void debugScene();


protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::list<PhysicsObject*> m_actors;

	PhysicsObject* pActor;

	PhysicsScene* m_physicsScene;
};

