#pragma once

#include <thread>
#include <atomic>
#include <chrono>

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

	void displayFPS();

private:

	DrawWindow m_window;

	static constexpr int m_world_size = 50;
	std::vector<Cube> cubes;

	Shader cube_shader;

	Axis x_axis = Axis(Axis_t::X);
	Axis y_axis = Axis(Axis_t::Y);
	Axis z_axis = Axis(Axis_t::Z);


	std::atomic<long long> m_fps = 0;
	std::atomic<bool> m_window_open = true;

};

bool Simulation::init()
{
	//initialise the window
	m_window.initialise();

	//iniitialise the projection matrix
	glm::mat4 projection = glm::perspective(
		glm::radians(m_window.getCamera().Zoom), 
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		0.1f, 
		100.0f);

	//timing the world creation
	auto begin_time = std::chrono::high_resolution_clock::now();

	cube_shader.init(
		"Data/shaders/vertex/vertex_shader.txt",
		"Data/shaders/fragment/fragment_shader.txt");

	//initialising the world
	for (int i = 0; i < m_world_size*m_world_size; i++)
	{
		cubes.emplace_back();
		cubes[i].initialise(projection, cube_shader);
		cubes[i].setPosition({i / m_world_size, 0, i % m_world_size});
	}

	//ending the world creation time
	auto end_time = std::chrono::high_resolution_clock::now();
	m_fps = std::chrono::duration<long long, std::nano>(end_time - begin_time).count();

	printf("Creation time: ");
	std::cout << std::to_string(m_fps/1000000000.f) << std::endl;

	//timing the initialisation of the axes
	begin_time = std::chrono::high_resolution_clock::now();
	x_axis.addFaces(cubes);
	y_axis.addFaces(cubes);
	z_axis.addFaces(cubes);
	end_time = std::chrono::high_resolution_clock::now();
	m_fps = std::chrono::duration<long long, std::nano>(end_time - begin_time).count();

	printf("Axes splitting time: ");
	std::cout << std::to_string(m_fps/1000000000.f) << std::endl;
	printf("\n");

	return true;
}

void Simulation::displayFPS()
{
	while (m_window_open) 
	{
		std::string display_text = "\x1b[1A\x1b[2K";
		display_text += std::to_string(1000000000.f/m_fps);
		std::cout  << display_text << std::endl;
	}
}

void Simulation::run()
{
	std::thread fps_thread([this] { displayFPS(); });

	while (m_window.open())
	{
		auto begin_time = std::chrono::high_resolution_clock::now();

		m_window.pollEvents();

		update();

		m_window.clear();

		render();

		m_window.display();

		auto end_time = std::chrono::high_resolution_clock::now();

		//Getting the number of nanoseconds that have passed
		m_fps = std::chrono::duration<long long, std::nano>(end_time - begin_time).count();
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