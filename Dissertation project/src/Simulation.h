#pragma once

#include "Window/WindowBase.h"
#include "Window/DrawWindow.h"
#include "shapes/triangle.h"
#include "Axis.h"

class Simulation
{
public:

	Simulation()
	{
	}

	bool init();
	void run();

private:

	void update();

	void render();

private:

	DrawWindow m_window;

	Cube test_cube;

	Axis x_axis = Axis(Axis_t::X);
	Axis y_axis = Axis(Axis_t::Y);
	Axis z_axis = Axis(Axis_t::Z);

};

bool Simulation::init()
{
	m_window.initialise();

	glm::mat4 projection = glm::perspective(glm::radians(m_window.getCamera().Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	test_cube.initialise(projection);
	test_cube.setPosition({ 0,0,2 });

	x_axis.addFaces({ test_cube });
	y_axis.addFaces({ test_cube });
	z_axis.addFaces({ test_cube });

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
	m_window.draw(x_axis);
	m_window.draw(y_axis);
	m_window.draw(z_axis);
}