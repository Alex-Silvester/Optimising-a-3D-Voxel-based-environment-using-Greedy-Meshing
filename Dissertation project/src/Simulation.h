#pragma once

#include "Window/WindowBase.h"
#include "Window/DrawWindow.h"
#include "shapes/triangle.h"

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

};

bool Simulation::init()
{
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
	//m_window.draw(test_triangle);
}