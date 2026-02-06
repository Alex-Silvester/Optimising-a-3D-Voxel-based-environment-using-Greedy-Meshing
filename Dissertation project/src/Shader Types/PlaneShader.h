#pragma once

#include "shader.h"

class PlaneShader : public Shader
{
public:
	PlaneShader()
	{
		Shader::init(
			"Data/shaders/vertex/vertex_shader.vert",
			"Data/shaders/fragment/Billboard_shader.frag");
	}

	void init()
	{
		Shader::init(
			"Data/shaders/vertex/vertex_shader.vert",
			"Data/shaders/fragment/billboard_shader.frag");
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

	PlaneShader &setProjection(const glm::mat4 &proj)
	{
		setMat4("projection", proj);

		return *this;
	}
};