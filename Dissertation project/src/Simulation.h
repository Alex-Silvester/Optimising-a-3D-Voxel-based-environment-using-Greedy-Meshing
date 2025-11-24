#pragma once

#include <thread>
#include <atomic>

#include "Timer.h"

#include "Window/WindowBase.h"
#include "Window/DrawWindow.h"
#include "shapes/triangle.h"
#include "Axis.h"

class Simulation
{
public:

	Simulation()
	{
		projection = glm::perspective(
			glm::radians(m_window.getCamera().Zoom),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			0.1f,
			100.0f);
	}

	bool init();
	void run();

private:

	void update();

	void render();

	void displayFPS()
	{
		while (m_window_open)
		{
			std::string display_text = "\x1b[1A\x1b[2K";
			display_text += std::to_string(1.f / m_fps);
			std::cout << display_text << std::endl;
		}
	}

	inline void worldCreation()
	{
		//timing the world creation
		Timer<std::nano> timer;
		timer.Start();

		cube_shader.init(
			"Data/shaders/vertex/vertex_shader.txt",
			"Data/shaders/fragment/fragment_shader.txt");

		//initialising the world
		for (int i = 0; i < m_world_size.x * m_world_size.y * m_world_size.z; i++)
		{
			cubes.emplace_back(
				projection, 
				glm::vec3{ i / (m_world_size.y * m_world_size.z),(i / m_world_size.z) % m_world_size.y,i % m_world_size.z },
				&cube_shader);
		}

		//ending the world creation time
		printf("Creation time: ");
		std::cout << std::to_string(timer.End()) << std::endl;
	}

	inline void axesSplitting()
	{
		Timer<std::nano> timer;

		//timing the initialisation of the axes
		timer.Start();
		x_axis.addFaces(cubes);
		y_axis.addFaces(cubes);
		z_axis.addFaces(cubes);

		printf("Axes splitting time: ");
		std::cout << std::to_string(timer.End()) << std::endl;
		printf("\n");
	}

private:

	DrawWindow m_window;

	static constexpr glm::vec<3, int> m_world_size = {3,3,3};
	std::vector<Cube> cubes;

	Shader cube_shader;

	Axis x_axis = Axis(Axis_t::X, m_world_size.x, m_world_size.y * m_world_size.z);
	Axis y_axis = Axis(Axis_t::Y, m_world_size.y, m_world_size.z * m_world_size.x);
	Axis z_axis = Axis(Axis_t::Z, m_world_size.z, m_world_size.x * m_world_size.y);


	std::atomic<double> m_fps = 0;
	std::atomic<bool> m_window_open = true;

	glm::mat4 projection;
};

bool Simulation::init()
{
	//initialise the window
	m_window.initialise();

	worldCreation();

	axesSplitting();

	return true;
}

void Simulation::run()
{
	//create a separate thread for the FPS count to not 
	// interfere too much with the simulation loop
	std::thread fps_thread([this] { displayFPS(); });

	Timer<std::nano> timer;
	while (m_window.open())
	{
		timer.Start();

		m_window.pollEvents();

		update();

		m_window.clear();

		render();

		m_window.display();

		//Getting the number of nanoseconds that have passed
		m_fps = timer.End();
	}
	m_window_open = false;

	fps_thread.join();
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