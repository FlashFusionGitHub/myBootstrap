#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include <vector>

class RotationalForceApp : public aie::Application {
public:

	RotationalForceApp();
	virtual ~RotationalForceApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	std::vector<Sphere*> spheres;
	std::vector<Box*> boxes;

	float m_position = 0;

	Box* box;
	Box* box2;

	Plane* plane;
	Plane* plane2;
	Plane* plane3;
	Plane* plane4;
};