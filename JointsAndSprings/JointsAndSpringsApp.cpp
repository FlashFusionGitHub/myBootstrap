#include "JointsAndSpringsApp.h"
#include "Texture.h"
#include "Font.h"
#include <Gizmos.h>
#include <glm/ext.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

JointsAndSpringsApp::JointsAndSpringsApp() {

}

JointsAndSpringsApp::~JointsAndSpringsApp() {

}

bool JointsAndSpringsApp::startup() {
	
	//increase the 2D line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();

	m_physicsScene->setGravity(glm::vec2(0.0f, -9.8f));

	//randomShapes();
	boxTest();
	springTest();

	ball = new Sphere(glm::vec2(150, 50), glm::vec2(), 10, 5, 0, glm::vec4(1, 0, 0, 1), 0, 0, 1);

	plane = new Plane(glm::vec2(1, 0), 10);
	plane2 = new Plane(glm::vec2(1, 0), 190);
	plane3 = new Plane(glm::vec2(0, 1), 10);
	plane4 = new Plane(glm::vec2(0, 1), 103);

	m_physicsObject.push_back(ball);

	m_physicsScene->addActor(plane);
	m_physicsScene->addActor(plane2);
	m_physicsScene->addActor(plane3);
	m_physicsScene->addActor(plane4);

	m_physicsScene->addActor(ball);

	return true;
}

void JointsAndSpringsApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void JointsAndSpringsApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	if (!isPaused) {

		m_physicsScene->update(deltaTime);

		//spawnBoxes(deltaTime);
		//spawnCircles(deltaTime);

	}

	m_physicsScene->updateGizmos();


	//Whacking Stick with left mouse button
	bool mouseDown = input->isMouseButtonDown(0);

	m_mousePoint = glm::vec2((float)input->getMouseX() / (float)getWindowWidth() * (float)200, (float)input->getMouseY() / (float)getWindowHeight() * (float)110);

	if (mouseDown != m_mouseDown) {
		if (mouseDown) {
			m_contactPoint = m_mousePoint;
		}
		else {
			for (auto it = m_physicsObject.begin(); it != m_physicsObject.end(); it++) {
				PhysicsObject* obj = *it;

				if (obj->isInside(m_mousePoint)) {
					Rigidbody* rb = (Rigidbody*)obj;
					rb->applyForce(10.0f * (m_mousePoint - m_contactPoint), m_contactPoint - rb->getPosition());
				}
			}
		}
		m_mouseDown = mouseDown;
	}

	// Move Objects with right mouse button
	for (auto it = m_physicsObject.begin(); it != m_physicsObject.end(); it++) {
		PhysicsObject* obj = *it;

		Rigidbody* rb = (Rigidbody*)obj;

		if (obj->isInside(m_mousePoint) && input->isMouseButtonDown(1)) {
			rb->setPosition(glm::vec2((float)input->getMouseX() / (float)getWindowWidth() * (float)200, (float)input->getMouseY() / (float)getWindowHeight() * (float)110));
			rb->setVelocity(glm::vec2(0, 0));

			float currentRotation = rb->getRotation();

			if (input->isKeyDown(aie::INPUT_KEY_Q)) {
				rb->setRotation(currentRotation -= deltaTime * 2.0f);
			}
			if (input->isKeyDown(aie::INPUT_KEY_E)) {
				rb->setRotation(currentRotation += deltaTime * 2.0f);
			}

		}
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	if (input->wasKeyPressed(aie::INPUT_KEY_GRAVE_ACCENT)) {
		isPaused = !isPaused;
	}
}

void JointsAndSpringsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.0f;

	if (m_mouseDown) {
		aie::Gizmos::add2DLine(m_contactPoint, m_mousePoint, glm::vec4(1, 1, 1, 1));
	}

	aie::Gizmos::draw2D(glm::ortho<float>(0, 200, 0 / aspectRatio, 200 / aspectRatio, -1.0f, 1.0f));

	if (isPaused) {
		m_2dRenderer->setRenderColour(1, 1, 1, 1);
		m_2dRenderer->drawText(m_font, "PAUSED", 10, 10);
	}

	// done drawing sprites
	m_2dRenderer->end();
}

void JointsAndSpringsApp::randomShapes()
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

void JointsAndSpringsApp::boxTest()
{
	m_physicsScene->setTimeStep(0.01f);

	box1 = new Box(glm::vec2(50, 90), glm::vec2(0, 0), 10, 10, 10, 0, glm::vec4(1, 0, 0, 1), 0, 0, 0);
	box2 = new Box(glm::vec2(50, 70), glm::vec2(0, 0), 10, 10, 10, 0, glm::vec4(1, 1, 1, 1), 0, 0, 0);
	box3 = new Box(glm::vec2(50, 50), glm::vec2(0, 0), 10, 10, 10, 0, glm::vec4(1, 0, 0, 1), 0, 0, 0);
	box4 = new Box(glm::vec2(50, 30), glm::vec2(0, 0), 10, 10, 10, 0, glm::vec4(1, 1, 1, 1), 0, 0, 0);

	box4->setKinematic(true);

	m_physicsObject.push_back(box1);
	m_physicsObject.push_back(box2);
	m_physicsObject.push_back(box3);
	m_physicsObject.push_back(box4);

	m_physicsScene->addActor(box1);
	m_physicsScene->addActor(box2);
	m_physicsScene->addActor(box3);
	m_physicsScene->addActor(box4);

}

void JointsAndSpringsApp::spawnBoxes(float deltaTime)
{
	m_timer += deltaTime;

	if (boxCount >= 3)
		return;

	glm::vec4 randomColour = glm::vec4(rand() % 2, rand() % 2, rand() % 2, 1);

	Box* spawnBox = new Box(glm::vec2(50, 90), glm::vec2(0, 0), 10, 10, 10, 0, glm::vec4(randomColour), 0, 0, 0);

	if (m_timer >= 2.0f) {
		m_timer = m_resetTimer;
		boxCount += 1;
		m_physicsObject.push_back(spawnBox);
		m_physicsScene->addActor(spawnBox);
	}
}

void JointsAndSpringsApp::spawnCircles(float deltaTime)
{
	if (circleCount >= 10)
		return;

	m_timer += deltaTime;

	glm::vec4 randomColour = glm::vec4(rand() % 2, rand() % 2, rand() % 2, 1);

	spawnedSphere = new Sphere(glm::vec2(50, 50), glm::vec2(), 5, 5, 0, randomColour, 0, 0, 1);

	if (m_timer >= 3.0f) {
		m_timer = m_resetTimer;
		circleCount += 1;
		m_physicsObject.push_back(spawnedSphere);
		m_physicsScene->addActor(spawnedSphere);
	}
}

void JointsAndSpringsApp::springTest()
{
	Sphere* ball = new Sphere(glm::vec2(70, 60), glm::vec2(), 10, 5, 0, glm::vec4(1, 1, 1, 1), 0, 0, 0);
	Sphere* ball2 = new Sphere(glm::vec2(70, 40), glm::vec2(), 10, 5, 0, glm::vec4(1, 1, 1, 1), 0, 0, 0);
	spring = new Spring(ball, ball2, 5, 0.5f, 1);

	ball->setKinematic(true);

	m_physicsObject.push_back(ball);
	m_physicsObject.push_back(ball2);

	m_physicsScene->addActor(ball);
	m_physicsScene->addActor(ball2);
	m_physicsScene->addActor(spring);
}