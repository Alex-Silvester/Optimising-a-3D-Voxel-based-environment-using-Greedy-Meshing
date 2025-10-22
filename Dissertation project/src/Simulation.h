#pragma once

#include "Window/WindowBase.h"
#include "Window/DrawWindow.h"
#include "shapes/triangle.h"
#include "shapes/Rect.h"

class Simulation
{
public:

	Simulation()
	{
	}

	bool init();
	void run();

private:

	void initialise();

	void update();

	void render();

private:

	DrawWindow m_window;

	Triangle test_triangle;
	Rect test_rect;

};

bool Simulation::init()
{
	m_window.initialise();

	glm::mat4 projection = glm::perspective(glm::radians(m_window.getCamera().Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	test_triangle.initialise(projection);
	test_triangle.setPosition({ 0,0,1 });

	test_rect.initialise(projection);
	test_rect.setPosition({ 0,2,1 });
	test_rect.setFacing(Axis::Z);

	return true;
}

void Simulation::run()
{
	while (m_window.open())
	{
		m_window.pollEvents();

		update();

		m_window.clear();

		render();

		m_window.display();
	}
}

void Simulation::update()
{

}

void Simulation::render()
{
	m_window.draw(test_triangle);

	m_window.draw(test_rect);
}