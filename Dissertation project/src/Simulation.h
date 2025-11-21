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

	std::vector<Cube> cubes;

	Axis x_axis = Axis(Axis_t::X);
	Axis y_axis = Axis(Axis_t::Y);
	Axis z_axis = Axis(Axis_t::Z);

	const int m_world_size = 50;

};

bool Simulation::init()
{
	m_window.initialise();

	glm::mat4 projection = glm::perspective(
		glm::radians(m_window.getCamera().Zoom), 
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		0.1f, 
		100.0f);

	auto begin_time = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < m_world_size*m_world_size; i++)
	{
		cubes.emplace_back();
		cubes.back().initialise(projection);
		cubes.back().setPosition({ i/m_world_size, 0, i%m_world_size });
	}
	x_axis.addFaces(cubes);
	y_axis.addFaces(cubes);
	z_axis.addFaces(cubes);

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