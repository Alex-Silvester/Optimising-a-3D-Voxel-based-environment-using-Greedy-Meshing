#pragma once

#include "../Interfaces/IDrawable.h"

enum Axis
{
	X = 0b001,
	Y = 0b010,
	Z = 0b100
};

class Rect : public IDrawable
{
public:

	void initialise(glm::mat4& projection) override
	{ 
		//left = positive-x
		//up = positive-y
		//back = positive-z

		//1.0 parts wide
		setVertices({
				0.5f, 0.5f, 0.f,   1.f, 0.f, 0.f,   0.f, 0.f, 0.f, 
			 -0.5f, 0.5f, 0.f,   0.f, 1.f, 0.f,   0.f, 0.f, 0.f, 
			 -0.5f,-0.5f, 0.f,   0.f, 0.f, 1.f,   0.f, 0.f, 0.f, 

			 -0.5f,-0.5f, 0.f,   0.f, 0.f, 1.f,   0.f, 0.f, 0.f,
				0.5f,-0.5f, 0.f,   0.f, 1.f, 0.f,   0.f, 0.f, 0.f,
				0.5f, 0.5f, 0.f,   1.f, 0.f, 0.f,   0.f, 0.f, 0.f
			});

		setShader(
			"Data/shaders/vertex/vertex_shader.txt",
			"Data/shaders/fragment/fragment_shader.txt",
			projection);
	}

	struct axis_type
	{
		axis_type(Axis axis)
		{
				x = ~(axis % 2);
			y = ~((axis >> 1) % 2);
			z = ~((axis >> 2) % 2);
		}
		
		unsigned int x : 1;
		unsigned int y : 1;
		unsigned int z : 1;
	};

	void setFacing(Axis facing_axis)
	{
		axis_type axis = facing_axis;

		

		setVertices({
					0.5f * axis.x,   0.5f * axis.y,  (axis.x ? 0.5f : -0.5f)* axis.z,   1.f, 0.f, 0.f,   0.f, 0.f, 0.f,
				 -0.5f * axis.x,   0.5f * axis.y,  0.5f                   * axis.z,   0.f, 1.f, 0.f,   0.f, 0.f, 0.f,
				 -0.5f * axis.x,  -0.5f * axis.y,  (axis.x ? -0.5f : 0.5f)* axis.z,   0.f, 0.f, 1.f,   0.f, 0.f, 0.f,
																			 
				 -0.5f * axis.x,  -0.5f * axis.y, (axis.x ? -0.5f :  0.5f)* axis.z,   0.f, 0.f, 1.f,   0.f, 0.f, 0.f,
					0.5f * axis.x,  -0.5f * axis.y, -0.5f * axis.z,                      0.f, 1.f, 0.f,   0.f, 0.f, 0.f,
					0.5f * axis.x,   0.5f * axis.y, (axis.x ?  0.5f : -0.5f)* axis.z,   1.f, 0.f, 0.f,   0.f, 0.f, 0.f
			});
	}

private:

};