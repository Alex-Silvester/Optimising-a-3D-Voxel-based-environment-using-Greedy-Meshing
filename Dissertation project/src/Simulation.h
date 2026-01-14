#pragma once

#include <thread>
#include <atomic>
#include <queue>

#include "Timer.h"

#include "Window/WindowBase.h"
#include "Window/DrawWindow.h"
#include "Axis.h"
#include "Noise/PerlinNoise.h"

#include "Helpers/Settings.h"

#include <cmath>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <ostream>
#include <print>
#include <ratio>
#include <string>
#include <vector>

#include "Shader Types/CubeShader.h"
#include "Shader Types/HUDShader.h"
#include "shapes/Cube.h"
#include "shapes/Rect.h"

#include "imgui-1.92.5/imgui.h"
#include "imgui-1.92.5/backends/imgui_impl_glfw.h"
#include "imgui-1.92.5/backends/imgui_impl_opengl3.h"

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

	void displayFPS()
	{
		for(int i = 0; i < AVERAGE_FPS_SAMPLES; i++)
		{
			fps_values.emplace(0);
		}

		while (m_window_open)
		{
			if (m_fps == 0) continue;

			time_passed += m_fps;
			sample_timer += m_fps;

			if(sample_timer > FPS_SAMPLE_SPACING)
			{
				average_fps = 0;
				sample_timer = 0;

				fps_values.pop();

				fps_values.emplace(1.f / m_fps);

				std::queue<float> temp_queue = fps_values;

				for (int i = 0; i < AVERAGE_FPS_SAMPLES; i++)
				{
					average_fps += temp_queue.front();
					temp_queue.pop();
				}
				average_fps /= AVERAGE_FPS_SAMPLES;
			}

			std::println("\x1b[1A\x1b[2KFPS: {} Average FPS: {} Time: {}", (int)(1.f/m_fps), (int)average_fps, (int)time_passed);

			//reset the fps value to check when the main loop has completed a frame
			m_fps = 0;
		}
	}

	inline void worldCreation();

	inline void axesSplitting();

	float magnitudeSqaured(const glm::vec3 &vector)
	{
		return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
	}

	void drawFaces();

	Rect *getFaceData();

	bool lineIntersectsQuad(Rect *quad, const glm::vec3 &line_start_pos, const glm::vec3& line_end_pos, glm::vec3& intersection_point);

	inline float scalarTriple(const glm::vec3 &u, const glm::vec3 &v, const glm::vec3 &w) const
	{
		return glm::dot(glm::cross(u, v), w);
	}

	Rect *getSelectedFace();

private:

	DrawWindow m_window;

	static constexpr glm::vec<3, int> m_world_size = {20, 20, 20};
	std::vector<Cube> cubes;

	CubeShader cube_shader;
	HUDShader hud_shader;

	Axis x_axis = Axis(Axis_t::X, m_world_size.x, m_world_size.y * m_world_size.z);
	Axis y_axis = Axis(Axis_t::Y, m_world_size.y, m_world_size.z * m_world_size.x);
	Axis z_axis = Axis(Axis_t::Z, m_world_size.z, m_world_size.x * m_world_size.y);

	std::atomic<double> m_fps = 0;
	std::atomic<bool> m_window_open = true;

	float m_world_creation_time = 0.f;
	float m_axes_split_time = 0.f;

	glm::mat4 projection;

	std::mutex mtx;

	std::queue<float> fps_values;
	float average_fps = 0;
	double time_passed = 0;
	double sample_timer = 0;

#if FACE_CHECKING == true
	glm::vec3 m_selected_position = {0,0,0};
	glm::vec3 m_selected_scale = { 0,0,0 };
	glm::vec3 m_selected_center = { 0,0,0 };
#endif

	Rect crosshair;

#if (COLLECT_FACES | OCCLUSION_CULL_QUERY) == true
	std::vector<Rect *> faces = {};
#endif
};

bool Simulation::init()
{
	//initialise the window
	m_window.initialise(1080, 720, "window");

	int size_x;
	int size_y;

	glfwGetWindowSize(m_window.getWindow(), &size_x, &size_y);

	projection = glm::perspective(
		glm::radians(m_window.getCamera().Zoom),
		(float)size_x / (float)size_y,
		0.1f,
		100.0f);


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
	cube_shader.setWorldSize(m_world_size);

	hud_shader.init();
	crosshair.initialise(projection, hud_shader.shaderPtr());
	crosshair.setColor({ 1,1,1 });
	crosshair.setFacing(Axis_t::Z);
	crosshair.scale({ 10.f / 1080.f, 10.f / 720.f, 1.f });

	glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return true;
}

void Simulation::run()
{
	//use this thread if imgui isn't used to get mildly more accurate fps measurements
#if USE_IMGUI == false
	//create a separate thread for the FPS count to not 
	// interfere too much with the simulation loop
	std::thread fps_thread([this] { displayFPS(); });
#else
	const char *glsl_version = "#version 330 core";

	float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup scaling
	ImGuiStyle &style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)


	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_window.getWindow(), true);

	ImGui_ImplOpenGL3_Init(glsl_version);

	for (int i = 0; i < AVERAGE_FPS_SAMPLES; i++)
	{
		fps_values.emplace(0);
	}
#endif

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

#if USE_IMGUI == false
	fps_thread.join();
#else
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
#endif
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

#if USE_IMGUI == true

	time_passed += m_fps;
	sample_timer += m_fps;

	if (sample_timer > FPS_SAMPLE_SPACING)
	{
		average_fps = 0;
		sample_timer = 0;

		fps_values.pop();

		fps_values.emplace(1.f / m_fps);

		std::queue<float> temp_queue = fps_values;

		for (int i = 0; i < AVERAGE_FPS_SAMPLES; i++)
		{
			average_fps += temp_queue.front();
			temp_queue.pop();
		}
		average_fps /= AVERAGE_FPS_SAMPLES;
	}

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Stats");

	ImGui::Text("Creation time: %.3f", m_world_creation_time);
	ImGui::Text("Axis optimisation time: %.3f", m_axes_split_time);

	ImGui::Text("FPS: %.f", 1.f / m_fps);
	ImGui::Text("Average FPS: %.f", average_fps);

#if FACE_CHECKING == true
	Rect *current_face = getSelectedFace();
	if (ImGui::Button("Get Face Data")  && current_face != nullptr)
	{
		m_selected_position = current_face->getPosition();
		m_selected_scale = current_face->getScale();
		m_selected_center = current_face->getCenter();
	}
	ImGui::Text("Position: [%.1f, %.1f, %.1f]", m_selected_position.x, m_selected_position.y, m_selected_position.z);
	ImGui::Text("Center: [%.1f, %.1f, %.1f]", m_selected_center.x, m_selected_center.y, m_selected_center.z);
	ImGui::Text("Scale: [%.1f, %.1f, %.1f]", m_selected_scale.x, m_selected_scale.y, m_selected_scale.z);
#endif
	ImGui::End();
#endif

#if FACE_CHECKING == true
	if (current_face != nullptr)
	{
		current_face->setColor({ 1.f,0.f,0.f });
	}
#endif
}

void Simulation::render()
{
#if Z_BUFFER_PRE_PASS == true

	// z-prepass
	//glEnable(GL_DEPTH_TEST);  // We want depth test !
	glDepthFunc(GL_LESS);   // We want to get the nearest pixels
	glColorMask(0, 0, 0, 0);  // Disable color, it's useless, we only want depth.
	glDepthMask(GL_TRUE);     // Ask z writing

	drawFaces();

	// real render
	//glEnable(GL_DEPTH_TEST);  // We still want depth test
	glDepthFunc(GL_LEQUAL);   // EQUAL should work, too. (Only draw pixels if they are the closest ones)
	glColorMask(1, 1, 1, 1);  // We want color this time
	glDepthMask(GL_FALSE);    // Writing the z component is useless now, we already have it

	drawFaces();

	glDepthMask(GL_TRUE);

#else
	drawFaces();
#endif

#if USE_IMGUI == true
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

	m_window.draw(crosshair);
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

	m_world_creation_time = timer.End();

#if USE_IMGUI == false
	//ending the world creation time
	printf("Creation time: ");
	std::cout << std::to_string(m_world_creation_time) << std::endl;
#endif
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

	m_axes_split_time = timer.End();

#if USE_IMGUI == false
	printf("Axes splitting time: ");
	std::cout << std::to_string(m_axes_split_time) << std::endl;
	printf("\n");
#endif
}

inline void Simulation::drawFaces()
{
#if (COLLECT_FACES | OCCLUSION_CULL_QUERY) == true
	for (Rect *&face : faces)
	{
		m_window.draw(*face);
	}
#else
	m_window.draw(x_axis);
	m_window.draw(y_axis);
	m_window.draw(z_axis);
#endif
}

inline Rect *Simulation::getFaceData()
{
	float dist = 100.f;



	return nullptr;
}

inline bool Simulation::lineIntersectsQuad(Rect *quad, const glm::vec3 &line_start_pos, const glm::vec3 &line_end_pos, glm::vec3& intersection_point)
{
	const glm::vec3 &q = line_start_pos;
	const glm::vec3 &p = line_end_pos;

	std::array<glm::vec3, 4> corners = quad->getCorners();
	const glm::vec3 &a = corners[0];
	const glm::vec3 &b = corners[1];
	const glm::vec3 &c = corners[2];
	const glm::vec3 &d = corners[3];


	glm::vec3 pq = q - p;
	glm::vec3 pa = a - p;
	glm::vec3 pb = b - p;
	glm::vec3 pc = c - p;

	glm::vec3 m = glm::cross(pc, pq);
	float v = glm::dot(pa, m);
	if (v >= 0.0f)
	{
		float u = -glm::dot(pb, m);
		if (u < 0.0f) return false;
		float w = scalarTriple(pq,pb,pa);
		if (w < 0.0f) return false;

		float denom = 1.0f / (u + v + w);
		u *= denom;
		v *= denom;
		w *= denom;
		intersection_point = u * a + v * b + w * c;
	}
	else
	{
		glm::vec3 pd = d - p;

		float u = glm::dot(pd, m);
		if (u < 0.0f) return false;
		float w = scalarTriple(pq, pa, pd);
		if (w < 0.0f) return false;

		v = -v;

		float denom = 1.0f / (u + v + w);
		u *= denom;
		v *= denom;
		w *= denom;
		intersection_point = u * a + v * d + w * c;
	}

	return true;
}

inline Rect *Simulation::getSelectedFace()
{
	float line_dist = 20.f;
	glm::vec3 camera_pos = m_window.getCamera().Position;
	glm::vec3 line_end = camera_pos + m_window.getCamera().Front * line_dist;

	Rect *current_face = nullptr;
	glm::vec3 current_intersection_point = {INFINITE, INFINITE, INFINITE};
	glm::vec3 checking_intersection_point;

	for (Rect *face : x_axis.getFaces())
	{
		face->setColor({ 0.f,1.f,0.f });

		if (lineIntersectsQuad(face, camera_pos, line_end, checking_intersection_point))
		{
			if (current_face == nullptr)
			{
				current_face = face;
				current_intersection_point = checking_intersection_point;
				continue;
			}

			//if the distance to the new face is further than the current, then check the next face
			if (glm::distance(camera_pos, current_intersection_point) <
					glm::distance(camera_pos, checking_intersection_point))
			{
				continue;
			}

			current_face = face;
			current_intersection_point = checking_intersection_point;
		}
	}

	for (Rect *face : y_axis.getFaces())
	{
		face->setColor({ 0.f,1.f,0.f });

		if (lineIntersectsQuad(face, camera_pos, line_end, checking_intersection_point))
		{
			if (current_face == nullptr)
			{
				current_face = face;
				current_intersection_point = checking_intersection_point;
				continue;
			}

			//if the distance to the new face is further than the current, then check the next face
			if ( glm::distance(camera_pos, current_intersection_point) <
					glm::distance(camera_pos, checking_intersection_point))
			{
				continue;
			}

			current_face = face;
			current_intersection_point = checking_intersection_point;
		}
	}

	for (Rect *face : z_axis.getFaces())
	{
		face->setColor({ 0.f,1.f,0.f });

		if (lineIntersectsQuad(face, camera_pos, line_end, checking_intersection_point))
		{
			if (current_face == nullptr)
			{
				current_face = face;
				current_intersection_point = checking_intersection_point;
				continue;
			}

			//if the distance to the new face is further than the current, then check the next face
			if (glm::distance(camera_pos, current_intersection_point) <
					glm::distance(camera_pos, checking_intersection_point))
			{
				continue;
			}

			current_face = face;
			current_intersection_point = checking_intersection_point;
		}
	}

	return current_face;
}
