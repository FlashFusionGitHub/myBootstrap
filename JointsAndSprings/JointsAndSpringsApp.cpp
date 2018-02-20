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

	ball = new Sphere(glm::vec2(150, 50), glm::vec2(), 10, 5, 0, glm::vec4(1, 0, 0, 1), 0, 0, 0.8);

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

	if (input->isMouseButtonDown(0)) {
		aie::Gizmos::add2DLine(glm::vec2(ball->getPosition()), glm::vec2((float)input->getMouseX() / (float)getWindowWidth() * (float)200, (float)input->getMouseY() / (float)getWindowHeight() * (float)110), glm::vec4(1, 1, 1, 1));
		ball->applyForce(glm::vec2((float)input->getMouseX() / (float)getWindowWidth() * (float)200, (float)input->getMouseY() / (float)getWindowHeight() * (float)110) - ball->getPosition(), glm::vec2());
	}

	if (input->isMouseButtonDown(1) && glm::distance(glm::vec2((float)input->getMouseX() / (float)getWindowWidth() * (float)200, (float)input->getMouseY() / (float)getWindowHeight() * (float)110), ball->getPosition()) < ball->getRadius()) {
		ball->setPosition(glm::vec2((float)input->getMouseX() / (float)getWindowWidth() * (float)200, (float)input->getMouseY() / (float)getWindowHeight() * (float)110));
		ball->setVelocity(glm::vec2(0, 0));
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	if (input->wasKeyPressed(aie::INPUT_KEY_GRAVE_ACCENT)) {
		isPaused = !isPaused;
		command = "";
	}

	if (isPaused) {
		if (input->wasKeyPressed(aie::INPUT_KEY_ENTER))
			execute(command);

		if (input->wasKeyPressed(aie::INPUT_KEY_BACKSPACE))
			command = command.substr(0, command.length() - 1);

		for (auto chars : input->getPressedCharacters()) {
			command += (char)chars;
		}
	}
}

void JointsAndSpringsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.0f;

	aie::Gizmos::draw2D(glm::ortho<float>(0, 200, 0 / aspectRatio, 200 / aspectRatio, -1.0f, 1.0f));

	if (isPaused) {
		float length = m_font->getStringWidth(arrow.c_str());

		m_2dRenderer->setRenderColour(1, 1, 1, 1);
		m_2dRenderer->drawText(m_font, arrow.c_str(), 10, 10);
		m_2dRenderer->drawText(m_font, command.c_str(), 10 + length, 10);
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

	//box1 = new Box(glm::vec2(50, 90), glm::vec2(0, 0), 10, 10, 10, 0, glm::vec4(1, 0, 0, 1), 0, 0, 0);
	//box2 = new Box(glm::vec2(50, 70), glm::vec2(0, 0), 10, 10, 10, 0, glm::vec4(1, 1, 1, 1), 0, 0, 0);
	//box3 = new Box(glm::vec2(50, 50), glm::vec2(0, 0), 10, 10, 10, 0, glm::vec4(1, 0, 0, 1), 0, 0, 0);
	box4 = new Box(glm::vec2(50, 30), glm::vec2(0, 0), 10, 10, 10, 0, glm::vec4(1, 1, 1, 1), 0, 0, 0);

	box4->setKinematic(true);

	//m_physicsScene->addActor(box1);
	//m_physicsScene->addActor(box2);
	//m_physicsScene->addActor(box3);
	m_physicsScene->addActor(box4);

}

void JointsAndSpringsApp::spawnBoxes(float deltaTime)
{
	m_timer += deltaTime;

	if (boxCount >= 7)
		return;

	glm::vec4 randomColour = glm::vec4(rand() % 2, rand() % 2, rand() % 2, 1);

	Box* spawnBox = new Box(glm::vec2(50, 90), glm::vec2(0, 0), 10, 10, 10, 0, glm::vec4(randomColour), 0, 0, 0);

	if (m_timer >= 2.0f) {
		m_timer = m_resetTimer;
		boxCount += 1;
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
		m_physicsScene->addActor(spawnedSphere);
	}

	//if (input->isMouseButtonDown(1) && glm::distance(glm::vec2((float)input->getMouseX() / (float)getWindowWidth() * (float)200, (float)input->getMouseY() / (float)getWindowHeight() * (float)110), spawnedSphere->getPosition()) < spawnedSphere->getRadius()) {
	//	spawnedSphere->setPosition(glm::vec2((float)input->getMouseX() / (float)getWindowWidth() * (float)200, (float)input->getMouseY() / (float)getWindowHeight() * (float)110));
	//	spawnedSphere->setVelocity(glm::vec2(0, 0));
	//}
}

void JointsAndSpringsApp::execute(std::string & command)
{
	aie::Input* input = aie::Input::getInstance();
	std::vector<std::string> commandParams = split(command, ' ');

	//logic

	command = "";
}

std::vector<std::string> JointsAndSpringsApp::split(const std::string string, char delim)
{
	std::string temp = "";

	std::vector<std::string> ret;

	for (char chars : string) {
		if (chars != delim)
			temp += chars;
		else {
			if (temp.length() > 0)
				ret.push_back(temp);
			temp.clear();
		}
	}
	ret.push_back(temp);

	return ret;
}