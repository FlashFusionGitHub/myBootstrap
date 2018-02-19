#include "RotationalForceApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include <glm/ext.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14159265359

RotationalForceApp::RotationalForceApp() {
	srand((unsigned)time(NULL));

	for (int i = 0; i < 100; i++) {
		rand();
	}
}

RotationalForceApp::~RotationalForceApp() {

}

bool RotationalForceApp::startup() {
	
	//increase the 2D line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();

	m_physicsScene->setGravity(glm::vec2(0.0f, -9.8f));

	//randomShapes();
	//boxTest();

	ball = new Sphere(glm::vec2(150, 50), glm::vec2(), 10, 5, 0, glm::vec4(1, 0, 0, 1), 0, 0, 0.1);

	plane = new Plane(glm::vec2(1, 0), 10);
	plane2 = new Plane(glm::vec2(1, 0), 190);
    plane3 = new Plane(glm::vec2(0, 1), 10);
	plane4 = new Plane(glm::vec2(0, 1), 103);

	m_physicsScene->addActor(plane);
	m_physicsScene->addActor(plane2);
	m_physicsScene->addActor(plane3);
	m_physicsScene->addActor(plane4);

	m_physicsScene->addActor(ball);

	return true;
}

void RotationalForceApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void RotationalForceApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	spawnBoxes(deltaTime);

	if (input->isMouseButtonDown(0)) {
	    aie::Gizmos::add2DLine(glm::vec2(ball->getPosition()), glm::vec2((float)input->getMouseX() / (float)getWindowWidth() * (float)200, (float)input->getMouseY() / (float)getWindowHeight() * (float)110), glm::vec4(1, 1, 1, 1));
		ball->applyForce(glm::vec2((float)input->getMouseX() / (float)getWindowWidth() * (float)200, (float)input->getMouseY() / (float)getWindowHeight() * (float)110) - ball->getPosition(), glm::vec2());
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void RotationalForceApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.0f;

	aie::Gizmos::draw2D(glm::ortho<float>(0, 200, 0 / aspectRatio, 200 / aspectRatio, -1.0f, 1.0f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void RotationalForceApp::randomShapes()
{
	m_physicsScene->setGravity(glm::vec2(0.0f, -9.8f));
	m_physicsScene->setTimeStep(0.01f);

	for (int i = 0; i < 10; i++) {
		spheres.push_back(new Sphere(glm::vec2(m_position += 15, 50), glm::vec2(rand() % 50 + 0, rand() % 50 + 0), 3.0f, 5.0f, 0, glm::vec4(rand() % 2, rand() % 2, rand() % 2, 1), 0.0f, 0.3f));
		m_physicsScene->addActor(spheres[i]);
		boxes.push_back(new Box(glm::vec2(m_position2 += 15, 30), glm::vec2(rand() % 50 + 0, rand() % 50 + 0), 3.0f, 8.0f, 8.0f, 40, glm::vec4(rand() % 2, rand() % 2, rand() % 2, 1), 0.0f, 0.3f));
		m_physicsScene->addActor(boxes[i]);
		spheres[i]->applyForce(glm::vec2(rand() % 50 + 0, rand() % 50 + 0), glm::vec2(20, 0));
	}
}

void RotationalForceApp::boxTest()
{
	m_physicsScene->setTimeStep(0.01f);

	box1 = new Box(glm::vec2(50, 90), glm::vec2(0, 0), 10, 10, 10, 0, glm::vec4(1, 0, 0, 1), 0, 0, 0);
	box2 = new Box(glm::vec2(50, 70), glm::vec2(0, 0), 10, 10, 10, 0, glm::vec4(1, 1, 1, 1), 0, 0, 0);
	box3 = new Box(glm::vec2(50, 50), glm::vec2(0, 0), 10, 10, 10, 0, glm::vec4(1, 0, 0, 1), 0, 0, 0);
	box4 = new Box(glm::vec2(50, 30), glm::vec2(0, 0), 10, 10, 10, 0, glm::vec4(1, 1, 1, 1), 0, 0, 0);
	m_physicsScene->addActor(box1);
	m_physicsScene->addActor(box2);
	m_physicsScene->addActor(box3);
	m_physicsScene->addActor(box4);
}

void RotationalForceApp::spawnBoxes(float deltaTime)
{
	m_timer += deltaTime;

	std::cout << m_timer << std::endl;

	glm::vec4 randomColour = glm::vec4(rand() % 2, rand() % 2, rand() % 2, 1);

	Box* spawnBox = new Box(glm::vec2(50, 90), glm::vec2(0, 0), 10, 10, 10, 0, glm::vec4(randomColour), 0, 0, 0);

	if (m_timer >= 3.0f) {
		m_timer = m_resetTimer;
		m_physicsScene->addActor(spawnBox);
	}
}


