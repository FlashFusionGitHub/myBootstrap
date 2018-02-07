#include "PhysicsScene.h"
#include "Rigidbody.h"
#include "Sphere.h"
#include "Plane.h"
#include <iostream>

//function pointer array for doing our collections
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] = {
	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere, PhysicsScene::plane2AABB,
	PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere, PhysicsScene::sphere2AABB,
	PhysicsScene::aabb2Plane, PhysicsScene::aabb2Sphere, PhysicsScene::aabb2AABB,
};

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
}


PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors) {
		delete pActor;
	}
}

void PhysicsScene::addActor(PhysicsObject * actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject * actor)
{
	m_actors.remove(actor);
}

void PhysicsScene::update(float deltaTime)
{
	static std::list<PhysicsObject*> dirty;

	//update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	while (accumulatedTime >= m_timeStep) {
		for (auto pActor : m_actors) {
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;

		//check for collisions (ideally you'd want to have some sort of scene management in place)
		checkForCollision();
	}
}

void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors) {
		pActor->makeGizmo();
	}
}

void PhysicsScene::debugScene()
{
	int count = 0;
	for (auto pActor : m_actors) {
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}
}

void PhysicsScene::checkForCollision()
{
	int actorCount = m_actors.size();

	//need to check for collision against all objects except this one
	for (auto it = m_actors.begin(); it != std::prev(m_actors.end()); it++) {
		for (auto it2 = std::next(it); it2 != m_actors.end(); it2++) {
			PhysicsObject* object1 = *it;
			PhysicsObject* object2 = *it2;
			int shapeId1 = object1->getShapeID();
			int shapeId2 = object2->getShapeID();

			//using function pointers
			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];

			if (collisionFunctionPtr != nullptr) {
				//did a collision occur?
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

bool PhysicsScene::plane2Plane(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::plane2AABB(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::sphere2Plane(PhysicsObject * obj1, PhysicsObject *obj2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(
			sphere->getPosition(),
			plane->getNormal()) - plane->getDistance();

		//if we are behind plane then we flip the normal
		if (sphereToPlane < 0) {
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->getRadius() - sphereToPlane;

		if (intersection > 0) {
			//set sphere velocity to zero here

			sphere->setVelocity(glm::vec2(0, 0));

			return true;
		}
	}

	return false;
}

bool PhysicsScene::sphere2AABB(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::aabb2AABB(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::aabb2Plane(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::aabb2Sphere(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::sphere2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//try to cast objects to sphere and sphere
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);
	//if we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr) {
		//this is where the collision detection happens
		//you need code which sets the velocity of the two spheres to zero
		//if they are overlapping
		// get distance between 2 spheres
		// if distance is less than the combined radius of
		// both spheres, then a collision occurred so set the
		// velocity of both spheres to 0 (we�ll add collision resolution later)
		if (glm::distance(sphere1->getPosition(), sphere2->getPosition()) < sphere1->getRadius() + sphere2->getRadius()) {
			sphere1->setVelocity(glm::vec2(0, 0));
			sphere2->setVelocity(glm::vec2(0, 0));
		}
	}

	return false;
}