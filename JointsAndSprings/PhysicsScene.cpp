#include "PhysicsScene.h"
#include "Rigidbody.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include <iostream>
#include <list>

//function pointer array for doing our collections
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] = {
	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere, PhysicsScene::plane2Box,
	PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere, PhysicsScene::sphere2Box,
	PhysicsScene::box2Plane, PhysicsScene::box2Sphere, PhysicsScene::box2Box,
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
	size_t actorCount = m_actors.size();

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

bool PhysicsScene::plane2Sphere(PhysicsObject *obj1, PhysicsObject *obj2)
{
	return sphere2Plane(obj2, obj1);
}

bool PhysicsScene::plane2Box(PhysicsObject *obj1, PhysicsObject *obj2)
{
	return box2Plane(obj2, obj1);
}

bool PhysicsScene::sphere2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//try to cast objects to sphere and sphere
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);

	//if we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr) {

		glm::vec2 delta = sphere2->getPosition() - sphere1->getPosition();
		float distance = glm::length(delta);
		float intersection = sphere1->getRadius() + sphere2->getRadius() - distance;

		//this is where the collision detection happens
		//you need code which sets the velocity of the two spheres to zero
		//if they are overlapping
		// get distance between 2 spheres
		// if distance is less than the combined radius of
		// both spheres, then a collision occurred so set the
		// velocity of both spheres to 0 (we’ll add collision resolution later)
		if (glm::distance(sphere1->getPosition(), sphere2->getPosition()) < sphere1->getRadius() + sphere2->getRadius()) {

			glm::vec2 contactForce = 0.5f * (distance - (sphere1->getRadius() + sphere2->getRadius())) * delta / distance;

			sphere1->setPosition(sphere1->getPosition() + contactForce);
			sphere2->setPosition(sphere2->getPosition() - contactForce);

			sphere1->resolveCollision(sphere2, 0.5f * (sphere1->getPosition() + sphere2->getPosition()));

			return true;
		}
	}

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
			glm::vec2 contact = sphere->getPosition() + (collisionNormal * -sphere->getRadius());
			plane->resolveCollision(sphere, contact);

			return true;
		}
	}

	return false;
}

bool PhysicsScene::sphere2Box(PhysicsObject *obj1, PhysicsObject *obj2)
{
	return box2Sphere(obj2, obj1);
}

bool PhysicsScene::box2Box(PhysicsObject *obj1, PhysicsObject *obj2)
{
	//try to cast objects to sphere and sphere
	Box* box = dynamic_cast<Box*>(obj1);
	Box* box2 = dynamic_cast<Box*>(obj2);

	if (box != nullptr && box2 != nullptr) {

		glm::vec2 boxPos = box2->getPosition() = box->getPosition();

		glm::vec2 norm(0, 0);
		glm::vec2 contactForce1, contactForce2;
		glm::vec2 contact(0, 0);
		int numContacts = 0;

		box->checkBoxCorners(*box2, contact, numContacts, norm, contactForce1);

		if (box2->checkBoxCorners(*box, contact, numContacts, norm, contactForce2)) {
			norm = -norm;
		}

		if (numContacts > 0) {
			glm::vec2 contactForce = 0.5f * (contactForce1 - contactForce2);
			box->setPosition(box->getPosition() - contactForce);
			box2->setPosition(box2->getPosition() + contactForce);

			box->resolveCollision(box2, contact / float(numContacts), &norm);


			return true;
		}
	}
	return false;
}

bool PhysicsScene::box2Plane(PhysicsObject *obj1, PhysicsObject *obj2)
{
	Box* box = dynamic_cast<Box*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	if (box != nullptr && plane != nullptr) {

		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;
		float radius = 0.5f * std::fminf(box->getWidth(), box->getHeight());
		float penetration = 0;

		glm::vec2 planeOrigin = plane->getNormal() * plane->getDistance();
		float comFromPlane = glm::dot(box->getPosition() - planeOrigin, plane->getNormal());

		for (float x = (-box->getWidth() / 2); x < box->getWidth(); x += box->getWidth()) {
			for (float y = (-box->getHeight() / 2); y < box->getHeight(); y += box->getHeight()) {

				glm::vec2 p = box->getPosition() + x * box->getLocalX() + y * box->getLocalY();

				float distFromPlane = glm::dot(p - planeOrigin, plane->getNormal());

				float velocityIntoPlane = glm::dot(box->getVelocity() + box->getAngularVelocity() * (-y * box->getLocalX() + x * box->getLocalY()), plane->getNormal());

				if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane > 0) || (distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane < 0)) {
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;

					if (comFromPlane >= 0) {
						if (penetration > distFromPlane)
							penetration = distFromPlane;
					}
					else {
						if (penetration < distFromPlane)
							penetration = distFromPlane;
					}
				}
			}
		}

		if (numContacts > 0) {

			float collisionV = contactV / (float)numContacts;

			glm::vec2 acceleration = -plane->getNormal() * ((1.0f + box->getElasticity()) * collisionV);

			glm::vec2 localContact = (contact / (float)numContacts) - box->getPosition();

			float r = glm::dot(localContact, glm::vec2(plane->getNormal().y, -plane->getNormal().x));

			float mass0 = 1.0f / (1.0f / box->getMass() + (r*r) / box->getMoment());

			box->applyForce(acceleration * mass0, localContact);

			box->setPosition(box->getPosition() - plane->getNormal() * penetration);
		}
	}

	return false;
}

bool PhysicsScene::box2Sphere(PhysicsObject *obj1, PhysicsObject *obj2)
{
	Box* box = dynamic_cast<Box*>(obj1);
	Sphere* sphere = dynamic_cast<Sphere*>(obj2);

	if (box != nullptr && sphere != nullptr) {

		glm::vec2 circlePos = sphere->getPosition() - box->getPosition();
		float w2 = box->getWidth() / 2, h2 = box->getHeight() / 2;

		int numContacts = 0;
		glm::vec2 contact(0, 0);

		for (float x = -w2; x <= w2; x += box->getWidth()) {
			for (float y = -h2; y <= h2; y += box->getHeight()) {
				glm::vec2 p = x * box->getLocalX() + y * box->getLocalY();
				glm::vec2 dp = p - circlePos;

				if (dp.x * dp.x + dp.y * dp.y < sphere->getRadius() * sphere->getRadius()) {
					numContacts++;
					contact += glm::vec2(x, y);
				}
			}
		}

		glm::vec2* direction = nullptr;

		glm::vec2 localPos(glm::dot(box->getLocalX(), circlePos), glm::dot(box->getLocalY(), circlePos));

		if (localPos.y < h2 && localPos.y > -h2) {
			if (localPos.x > 0 && localPos.x < w2 + sphere->getRadius()) {
				numContacts++;
				contact += glm::vec2(w2, localPos.y);
				direction = new glm::vec2(box->getLocalX());
			}
			if (localPos.x < 0 && localPos.x > -(w2 + sphere->getRadius())) {
				numContacts++;
				contact += glm::vec2(-w2, localPos.y);
				direction = new glm::vec2(-box->getLocalX());
			}
		}

		if (localPos.x < w2 && localPos.x > -w2) {
			if (localPos.y > 0 && localPos.y < h2 + sphere->getRadius()) {
				numContacts++;
				contact += glm::vec2(localPos.x, h2);
				direction = new glm::vec2(box->getLocalY());
			}
			if (localPos.y < 0 && localPos.y > -(h2 + sphere->getRadius())) {
				numContacts++;
				contact += glm::vec2(localPos.x, -h2);
				direction = new glm::vec2(-box->getLocalY());
			}
		}

		if (numContacts > 0) {
			contact = box->getPosition() + (1.0f / numContacts) * (box->getLocalX() * contact.x + box->getLocalY() * contact.y);

			float pen = sphere->getRadius() - glm::length(contact - sphere->getPosition());
			glm::vec2 penVec = glm::normalize(contact - sphere->getPosition()) * pen;

			// move each shape away in the direction of penitration 
			if (!box->isKinematic() && !sphere->isKinematic()) {
				box->setPosition(box->getPosition() + penVec*0.5f);
				sphere->setPosition(sphere->getPosition() - penVec*0.5f); 
			} 
			else if (!box->isKinematic()) {
				box->setPosition(box->getPosition() + penVec); 
			} 
			else {
				sphere->setPosition(sphere->getPosition() - penVec);
			}

			box->resolveCollision(sphere, contact, direction);
		}
		delete direction;
		return true;
	}
	return false;
}
