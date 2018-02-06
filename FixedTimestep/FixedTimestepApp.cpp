#include "FixedTimestepApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include <glm/ext.hpp>
#include <iostream>
#include "Sphere.h"

FixedTimestepApp::FixedTimestepApp() {

}

FixedTimestepApp::~FixedTimestepApp() {

}

bool FixedTimestepApp::startup() {

	//increase the 2D line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0.0f, -10.0f));
	m_physicsScene->setTimeStep(0.01f);

	ball1 = new Sphere(glm::vec2(0, 0), glm::vec2(0, 0), m_fuel, 8.0f, glm::vec4(1, 0, 0, 1));

	m_physicsScene->addActor(ball1);

	return true;
}

void FixedTimestepApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void FixedTimestepApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();


	if (m_fuel > 0.0f) {
		m_fuel -= weight;

		balls.push_back(new Sphere(glm::vec2(ball1->getPosition().x, ball1->getPosition().y - 10.0f), glm::vec2(0, 0), weight, 1.0f, glm::vec4(1, 0, 0, 1)));

		m_physicsScene->addActor(balls.back());

		ball1->applyForceToActor(balls.back(), glm::vec2(0, -30));

		for (auto it = balls.begin(); it != balls.end(); ++it)
		{
			if ((*it)->getPosition().y < (0 - 40 - (*it)->getRadius())) {
				m_physicsScene->removeActor((*it));
			}
		}
	}

	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void FixedTimestepApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!

	static float aspectRatio = 16 / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}