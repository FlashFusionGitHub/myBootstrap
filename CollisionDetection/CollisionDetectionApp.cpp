#include "CollisionDetectionApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include <glm/ext.hpp>

CollisionDetectionApp::CollisionDetectionApp() {

}

CollisionDetectionApp::~CollisionDetectionApp() {

}

bool CollisionDetectionApp::startup() {
	
	//increase the 2D line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0.0f, 0.0f));
	m_physicsScene->setTimeStep(0.01f);

	//plane1 = new Plane(glm::vec2(0.5f, 1.0f), 0.0f);

	ball1 = new Sphere(glm::vec2(-20, 20), glm::vec2(0, 0), 10.0f, 5.0f, glm::vec4(1, 0, 0, 1));
	ball2 = new Sphere(glm::vec2(20, 20), glm::vec2(0, 0), 10.0f, 5.0f, glm::vec4(1, 0, 0, 1));

	//aabb = new AABB(glm::vec2(20, 20), glm::vec2(0, 0), 10.0f, 5.0f, 5.0f, 0, glm::vec4(1, 1, 0, 1));
	//AABB* aabb2 = new AABB(glm::vec2(10, 10), glm::vec2(0, 0), 10.0f, 5.0f, 5.0f, 0, glm::vec4(1, 0, 0, 1));

	m_physicsScene->addActor(ball1);
	m_physicsScene->addActor(ball2);
	//m_physicsScene->addActor(aabb2);
	//m_physicsScene->addActor(plane1);

	ball2->applyForce(glm::vec2(-60, 0));


	return true;
}

void CollisionDetectionApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void CollisionDetectionApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();


	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void CollisionDetectionApp::draw() {

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