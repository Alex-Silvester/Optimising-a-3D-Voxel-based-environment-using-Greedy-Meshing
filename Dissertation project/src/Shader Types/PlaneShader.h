#pragma once

#include "shader.h"

class PlaneShader : public Shader
{
public:
	PlaneShader()
	{
		Shader::init(
			"Data/shaders/vertex/vertex_shader.vert",
			"Data/shaders/fragment/plane_shader.frag");

		use();
		setVec3("objectColor", 1.f, 1.f, 1.f);
		setVec3("lightColor", 1.f, 1.f, 1.f);
	}

	void init()
	{
		Shader::init(
			"Data/shaders/vertex/vertex_shader.vert",
			"Data/shaders/fragment/plane_shader.frag");
	}

	void use() const
	{
		this->Shader::use();
	}

	Shader *shaderPtr()
	{
		if (Shader *ptr = dynamic_cast<Shader *>(this))
		{
			return ptr;
		}
		return nullptr;
	}

	PlaneShader &setLightPosition(const glm::vec3 &pos)
	{
		setVec3("lightPos", pos);
		return *this;
	}

	PlaneShader &setLightIntensity(float intensity)
	{
		setFloat("intensity", intensity);
		return *this;
	}

	PlaneShader &setAmbientIntensity(float ambient)
	{
		setFloat("ambientStrength", ambient);
		return *this;
	}

	PlaneShader &setProjection(const glm::mat4 &proj)
	{
		setMat4("projection", proj);

		return *this;
	}
};