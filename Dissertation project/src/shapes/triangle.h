#pragma once

#include "../Interfaces/IDrawable.h"

class Triangle : public IDrawable
{
public:

	Triangle()
	{
		setVertices({
			  0.f,   0.f, 0.f,   1.f, 1.f, 1.f,   0.f, 0.f, 0.f,
			100.f,   0.f, 0.f,   1.f, 1.f, 1.f,   0.f, 0.f, 0.f,
			  0.f, 100.f, 0.f,   1.f, 1.f, 1.f,   0.f, 0.f, 0.f
			});

		setShader("Data/shaders/vertex/vertex_shader.txt", "Data/shaders/fragment/fragment_shader.txt");
	}

private:


};