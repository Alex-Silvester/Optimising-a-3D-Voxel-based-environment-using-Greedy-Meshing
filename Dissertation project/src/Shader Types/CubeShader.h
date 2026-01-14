#pragma once

#include "shader.h"

class CubeShader : private Shader
{
public:

	CubeShader() = default;

	void init()
	{
		Shader::init(
			"Data/shaders/vertex/vertex_shader.vert",
			"Data/shaders/fragment/fragment_shader.frag");
	}

	Shader* shaderPtr()
	{
		if (Shader* ptr = dynamic_cast<Shader*>(this))
		{
			return ptr;
		}
		return nullptr;
	}

	void setLightColor(float r, float g, float b)
	{
		setVec3("lightColor", r, g, b);
	}

	void setLightColor(const Color& col)
	{
		setVec3("lightColor", col.r, col.g, col.b);
	}

	void setLightPosition(float x, float y, float z)
	{
		setVec3("lightPos", glm::vec3(x, y, z));
	}

	void setLightPosition(const glm::vec3& pos)
	{
		setVec3("lightPos", pos);
	}

	template<typename T>
	void setLightIntensity(T intensity)
	{
		setFloat("intensity", intensity);
	}

	template<typename T>
	void setAmbientLightStrength(T ambient)
	{
		setFloat("ambientStrength", ambient);
	}

	void setWorldSize(const glm::vec3 &size)
	{
		setVec3("worldSize", size);
	}

private:

};