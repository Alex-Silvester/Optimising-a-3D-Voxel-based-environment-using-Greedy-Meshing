#pragma once

#include <cstddef>
#include <concepts>
#include <functional>
#include <string>
#include <type_traits>

#include "shader.h"

class CubeShader : private Shader
{
public:

	CubeShader()
	{
		Shader::init(
			"Data/shaders/vertex/vertex_shader.vert",
			"Data/shaders/fragment/fragment_shader.frag");
	}

	void init()
	{
		Shader::init(
			"Data/shaders/vertex/vertex_shader.vert",
			"Data/shaders/fragment/fragment_shader.frag");
	}

	void use() const
	{
		this->Shader::use();
	}

	Shader* shaderPtr()
	{
		if (Shader* ptr = dynamic_cast<Shader*>(this))
		{
			return ptr;
		}
		return nullptr;
	}

	CubeShader& setLightColor(float r, float g, float b)
	{
		setVec3("lightColor", r, g, b);

		return *this;
	}

	CubeShader& setLightColor(const Color& col)
	{
		setVec3("lightColor", col.r, col.g, col.b);

		return *this;
	}

	CubeShader& setLightPosition(float x, float y, float z)
	{
		setVec3("lightPos", glm::vec3(x, y, z));

		return *this;
	}

	CubeShader& setLightPosition(const glm::vec3& pos)
	{
		setVec3("lightPos", pos);

		return *this;
	}

	template<typename T> 
	CubeShader& setLightIntensity(T intensity)
	{
		setFloat("intensity", intensity);

		return *this;
	}

	template<typename T>
	CubeShader& setAmbientLightStrength(T ambient)
	{
		setFloat("ambientStrength", (float)ambient);

		return *this;
	}

	CubeShader& setWorldSize(const glm::vec3 &size)
	{
		setVec3("worldSize", size);

		return *this;
	}

	CubeShader& setObjectColor(float r, float g, float b)
	{
		setVec3("objectColor", r, g, b);

		return *this;
	}

	CubeShader& setObjectColor(const glm::vec3 &col)
	{
		setVec3("objectColor", col);

		return *this;
	}

	CubeShader& setProjection(const glm::mat4 &proj)
	{
		setMat4("projection", proj);

		return *this;
	}

private:

};