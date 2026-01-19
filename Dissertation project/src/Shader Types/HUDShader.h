#pragma once

#include "shader.h"

class HUDShader : public Shader
{
public:

	HUDShader()
	{
		Shader::init(
			"Data/shaders/vertex/HUD_shader.vert",
			"Data/shaders/fragment/HUD_shader.frag");
	}

	void init()
	{
		Shader::init(
			"Data/shaders/vertex/HUD_shader.vert",
			"Data/shaders/fragment/HUD_shader.frag");
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