#pragma once

#include <thread>
#include <atomic>

#include "Timer.h"

#include "Window/WindowBase.h"
#include "Window/DrawWindow.h"
#include "Axis.h"
#include "Noise/PerlinNoise.h"

#include "Helpers/Settings.h"

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

	inline void worldCreation();

	inline void axesSplitting();

	float magnitudeSqaured(const glm::vec3 &vector)
	{
		return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
	}

private:

	DrawWindow m_window;

	static constexpr glm::vec<3, int> m_world_size = {20,15,20};
	std::vector<Cube> cubes;

	CubeShader cube_shader;

	Axis x_axis = Axis(Axis_t::X, m_world_size.x, m_world_size.y * m_world_size.z);
	Axis y_axis = Axis(Axis_t::Y, m_world_size.y, m_world_size.z * m_world_size.x);
	Axis z_axis = Axis(Axis_t::Z, m_world_size.z, m_world_size.x * m_world_size.y);

	std::atomic<double> m_fps = 0;
	std::atomic<bool> m_window_open = true;

	glm::mat4 projection;

	std::mutex mtx;


#if (COLLECT_FACES | OCCLUSION_CULL_QUERY) == true
	std::vector<Rect *> faces = {};
#endif
};

bool Simulation::init()
{
	//initialise the window
	m_window.initialise();

	worldCreation();

	axesSplitting();

#if (COLLECT_FACES | OCCLUSION_CULL_QUERY) == true
	faces.resize(x_axis.getFaces().size() + y_axis.getFaces().size() + z_axis.getFaces().size());
	int i = 0;
	for (Rect *face : x_axis.getFaces())
	{
		faces[i] = std::move(face);
		i++;
	}
	for (Rect *face : y_axis.getFaces())
	{
		faces[i] = std::move(face);
		i++;
	}
	for (Rect *face : z_axis.getFaces())
	{
		faces[i] = std::move(face);
		i++;
	}
#endif

	cube_shader.setLightPosition(m_world_size.x / 2.f, m_world_size.y, m_world_size.z / 2.f);
	cube_shader.setAmbientLightStrength(0.5f);
	cube_shader.setLightIntensity(1.f);

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

		if (m_window.paused()) continue;

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
#if OCCLUSION_CULL_QUERY == true
	std::sort(faces.begin(), faces.end(), [this](Rect *face1, Rect *face2)
	{
		return
			magnitudeSqaured(face1->getPosition() - m_window.getCamera().Position) <
			magnitudeSqaured(face2->getPosition() - m_window.getCamera().Position);
	});
#endif
}

void Simulation::render()
{
#if COLLECT_FACES == true
	for(Rect*& face : faces)
	{
		m_window.draw(*face);
	}
#else
	m_window.draw(x_axis);
	m_window.draw(y_axis);
	m_window.draw(z_axis);
#endif
}

void Simulation::worldCreation()
{
	Noise::PerlinNoise& noise_gen = Noise::PerlinNoise::noise();

	//timing the world creation
	Timer<std::nano> timer;
	timer.Start();

	cube_shader.init();

	//initialising the world
	for (int i = 0; i < m_world_size.x * m_world_size.y * m_world_size.z; i++)
	{
		glm::vec3 pos = glm::vec3{ i / (m_world_size.y * m_world_size.z),(i / m_world_size.z) % m_world_size.y,i % m_world_size.z };

#if USE_NOISE
		float noise_eval = noise_gen.eval({ pos.x / m_world_size.x, pos.y / m_world_size.y, pos.z / m_world_size.z });
		noise_eval = std::fabsf(std::isnan(noise_eval) ? 0 : noise_eval);

		if (noise_eval > 0.2f) continue;
#endif

		cubes.emplace_back(projection, pos, cube_shader.shaderPtr());
	}

	//ending the world creation time
	printf("Creation time: ");
	std::cout << std::to_string(timer.End()) << std::endl;
}

inline void Simulation::axesSplitting()
{
	Timer<std::nano> timer;

	//timing the initialisation of the axes
	timer.Start();

	std::thread x_thread([this] { x_axis.addFaces(cubes, mtx); });
	std::thread y_thread([this] { y_axis.addFaces(cubes, mtx); });
	std::thread z_thread([this] { z_axis.addFaces(cubes, mtx); });

	x_thread.join();
	y_thread.join();
	z_thread.join();

	printf("Axes splitting time: ");
	std::cout << std::to_string(timer.End()) << std::endl;
	printf("\n");
}
