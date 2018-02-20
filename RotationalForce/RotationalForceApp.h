#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Input.h"
#include <vector>

class RotationalForceApp : public aie::Application {
public:

	RotationalForceApp();
	virtual ~RotationalForceApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void randomShapes();
	void boxTest();
	void spawnBoxes(float deltaTime);
	void spawnCircles(float deltaTime);

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	std::vector<Sphere*> spheres;
	std::vector<Box*> boxes;

	float m_position = 0;
	float m_position2 = 0;

	Plane* plane;
	Plane* plane2;
	Plane* plane3;
	Plane* plane4;

	Sphere* ball;
	Sphere* spawnedSphere;

	Box* box1;
	Box* box2;
	Box* box3;
	Box* box4;

	float m_timer = 0.0f;
	float m_resetTimer = 0.0f;

	int circleCount = 0;


};