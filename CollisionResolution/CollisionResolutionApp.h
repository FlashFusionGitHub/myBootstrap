#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include <vector>

class CollisionResolutionApp : public aie::Application {
public:

	CollisionResolutionApp();
	virtual ~CollisionResolutionApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	std::vector<Sphere*> spheres;

	float m_position = -80;

	Plane* plane;
	Plane* plane2;
	Plane* plane3;
	Plane* plane4;
};