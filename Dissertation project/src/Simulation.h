#pragma once

#include <glad/glad.h>
#include <glad.c>
#include <GLFW/glfw3.h>

#include <iostream>

constexpr auto SCREEN_WIDTH = 720;
constexpr auto SCREEN_HEIGHT = 540;

class Simulation
{
public:

	Simulation()
	{
		lastX = SCREEN_WIDTH / 2.0f;
		lastY = SCREEN_HEIGHT / 2.0f;
		first_mouse = true;
	}

	~Simulation() 
	{
		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();

		glfwDestroyWindow(window);
	}

	bool init();
	void run();

private:

	void initialise();

	void update();

	void render();

	void createWindow();

	void processInput();

	GLFWwindow* makeWindow(const char* name = "");

private:

	GLFWwindow* window = nullptr;

	// camera
	//Camera camera{ glm::vec3(0.f, 0.f, 0.f) , {0.f,1.f,0.f}, 90.f, 0.f };
	float lastX;
	float lastY;
	bool first_mouse;

};

bool Simulation::init()
{
	initialise();

	createWindow();

	return true;
}

void Simulation::run()
{
	while (!glfwWindowShouldClose(window))
	{
		update();

		render();
	}
}

void Simulation::initialise()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void Simulation::update()
{
	processInput();
}

void Simulation::render()
{
	// render
	// ------
	//glClearColor(0.5f, 0.8f, 0.9f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//keep these at the end
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Simulation::createWindow()
{
	window = makeWindow();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	//if (first_mouse)
	//{
	//	lastX = xpos;
	//	lastY = ypos;
	//	first_mouse = false;
	//}

	//float xoffset = xpos - lastX;
	//float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	//lastX = xpos;
	//lastY = ypos;

	//camera.ProcessMouseMovement(xoffset, yoffset);
}

GLFWwindow* Simulation::makeWindow(const char* name)
{
	glfwWindowHint(GLFW_SAMPLES, 4);
	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, name, NULL, NULL);
	if (window == NULL)
	{
		std::cout << ("Failed to create GLFW window") << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	return window;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Simulation::processInput()
{
	//GLint polygonMode;
	//glGetIntegerv(GL_POLYGON_MODE, &polygonMode);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	/*
	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS && !f5_pressed)
	{
		f5_pressed = true;
		if (polygonMode == GL_FILL)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else if (polygonMode == GL_LINE)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_RELEASE && f5_pressed)
	{
		f5_pressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && flying)
	{
		camera.ProcessKeyboard(UP, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !space_pressed)
	{
		space_pressed = true;
		camera.hitbox.grounded = false;
		camera.y_velocity = 10.f;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		space_pressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.sprint_active = true;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		camera.sprint_active = false;

	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !tab_pressed)
	{
		tab_pressed = true;
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
	{
		tab_pressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS && !alt_pressed)
	{
		alt_pressed = true;
		flying = !flying;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE)
	{
		alt_pressed = false;
	}
	*/

	//when breaking a block, use a separate thread for the checkForBlockUpdates function

}