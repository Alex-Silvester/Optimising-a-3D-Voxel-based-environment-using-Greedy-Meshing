#pragma once

#include "shader.h"

class BillboardShader : public Shader
{
public:

	BillboardShader()
	{
		Shader::init(
			"Data/shaders/vertex/Billboard_shader.vert",
			"Data/shaders/fragment/Billboard_shader.frag");
	}

	void init()
	{
		Shader::init(
			"Data/shaders/vertex/Billboard_shader.vert",
			"Data/shaders/fragment/Billboard_shader.frag");
	}

	void setProjection(glm::mat4 &projection)
	{
		setMat4("projection", projection);
	}

	void setAlpha(float alpha)
	{
		setFloat("alpha", alpha);
	}

	Shader *shaderPtr()
	{
		if (Shader *ptr = dynamic_cast<Shader *>(this))
		{
			return ptr;
		}
		return nullptr;
	}
};