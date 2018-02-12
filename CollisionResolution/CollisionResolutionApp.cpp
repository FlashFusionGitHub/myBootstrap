#include "CollisionResolutionApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include <glm/ext.hpp>
#include <iostream>
#include <time.h>

CollisionResolutionApp::CollisionResolutionApp() {

}

CollisionResolutionApp::~CollisionResolutionApp() {

}

bool CollisionResolutionApp::startup() {
	
	//increase the 2D line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	srand(time(NULL));

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0.0f, -9.8f));
	m_physicsScene->setTimeStep(0.01f);

	for (int i = 0; i < 9; i++) {
		spheres.push_back(new Sphere(glm::vec2(m_position += 15, 0), glm::vec2(0, 0), 10.0f, 5.0f, glm::vec4(1, 0, 0, 1)));
		m_physicsScene->addActor(spheres[i]);
		spheres[i]->applyForce(glm::vec2(rand() % 200 + 100, rand() % 200 + 100));
	}

	plane = new Plane(glm::vec2(1, 0), 90);
	plane2 = new Plane(glm::vec2(1, 0), -90);
	plane3 = new Plane(glm::vec2(0, 1), 50);
	plane4 = new Plane(glm::vec2(0, 1), -50);

	m_physicsScene->addActor(plane);
	m_physicsScene->addActor(plane2);
	m_physicsScene->addActor(plane3);
	m_physicsScene->addActor(plane4);

	return true;
}

void CollisionResolutionApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void CollisionResolutionApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	if (input->wasMouseButtonPressed(0)) {
		Sphere* sphere = new Sphere(glm::vec2(input->getMouseX(), input->getMouseY()), glm::vec2(0, 0), 10, 10, glm::vec4(1, 0, 0, 1));
		m_physicsScene->addActor(sphere);
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

glm::vec2 CollisionResolutionApp::screen2World(glm::vec2 screenPos)
{
	float width = 1280;
	float height = 720;

	glm:vec2 worldPos( screenPos/width)
}

void CollisionResolutionApp::draw() {

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