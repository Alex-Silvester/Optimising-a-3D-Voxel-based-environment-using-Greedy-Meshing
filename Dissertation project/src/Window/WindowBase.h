#pragma once

#include <glad/glad.h>
#include <glad.c>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

constexpr auto SCREEN_WIDTH = 720;
constexpr auto SCREEN_HEIGHT = 540;

class WindowBase
{
public:

	WindowBase() = default;

	WindowBase(float size_x, float size_y, const char* name = "")
	{
		if (!initialise())
		{
			std::cerr << "Failed to initialise WindowBase";
		}
	}

	~WindowBase()
	{
		glfwDestroyWindow(m_window);
	}

	bool open()
	{
		return !glfwWindowShouldClose(m_window);
	}

	void close()
	{
		glfwSetWindowShouldClose(m_window, true);
	}

	void clear()
	{
		glClearColor(0.5f, 0.8f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void display()
	{
		glfwSwapBuffers(m_window);

		float currentFrame = static_cast<float>(glfwGetTime());
		m_delta_time = currentFrame - m_last_frame;
		m_last_frame = currentFrame;
	}

	void pollEvents()
	{
		glfwPollEvents();

		processInput();
	}

protected:

	virtual bool initialise()
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

		m_window = createWindow();
		glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));


		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return false;
		}

		return true;
	}

private:

	GLFWwindow* createWindow()
	{
		GLFWwindow* window = makeWindow();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return window;
	}

	GLFWwindow* makeWindow(const char* name = "")
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

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		return window;
	}

	static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
	{
		WindowBase* user_window = reinterpret_cast<WindowBase*>(glfwGetWindowUserPointer(window));

		if (user_window)
		{
			user_window->mouseEvent(xposIn, yposIn);
		}
	}

	virtual void mouseEvent(double xposIn, double yposIn){}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}

	virtual void processInput()
	{
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(m_window, true);
		}
	}


protected:

	float m_delta_time = 0.f;

	GLFWwindow* m_window = nullptr;

private:


	float m_last_frame = 0.f;

};