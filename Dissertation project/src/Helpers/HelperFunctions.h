#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "../camera.h"

namespace hf
{
	glm::vec3 _1Dto3D(int _1D_coord, const glm::vec<3, int> & _3D_size)
	{
		return glm::vec3{ _1D_coord / (_3D_size.y * _3D_size.z),(_1D_coord / _3D_size.z) % _3D_size.y,_1D_coord % _3D_size.z };
	}

	glm::mat4 getProjection(GLFWwindow *window, const Camera &camera)
	{
		int size_x;
		int size_y;

		glfwGetWindowSize(window, &size_x, &size_y);

		return  glm::perspective(
			glm::radians(camera.Zoom),
			(float)size_x / (float)size_y,
			0.1f,
			100.0f);
	}
}