#pragma once

#include "../Interfaces/IDrawable.h"

class Triangle : public IDrawable
{
public:

	Triangle() = default;

	void initialise(glm::mat4& projection) override
	{
		//0.5 parts wide
		setVertices({
				 0.0f,  0.0f, 0.0f,   1.f, 0.0f, 0.0f,   0.f, 0.f, 0.f,
				 0.0f,  0.5f, 0.0f,   0.f, 1.f, 0.0f,   0.f, 0.f, 0.f,
				 0.5f,  0.0f, 0.0f,   0.f, 0.f, 1.f,   0.f, 0.f, 0.f
			});

		setShader(
			"Data/shaders/vertex/vertex_shader.txt", 
			"Data/shaders/fragment/fragment_shader.txt",
			projection);
	}

private:


};