#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include <list>

class FixedTimestepApp : public aie::Application {
public:

	FixedTimestepApp();
	virtual ~FixedTimestepApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	Sphere* ball1;
	std::list<Sphere*> balls;

	float m_fuel = 100.0f;
	float weight = 0.5f;
};