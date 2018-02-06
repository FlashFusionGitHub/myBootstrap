#pragma once

#include "Application.h"
#include "Renderer2D.h"

#include <Gizmos.h>

class physicsSimulation2DApp : public aie::Application {
public:

	physicsSimulation2DApp();
	virtual ~physicsSimulation2DApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
};