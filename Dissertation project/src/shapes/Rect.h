#pragma once

#include "../Interfaces/IDrawable.h"

enum Axis_t
{
	EMPTY = 0,
	X = 0b001,
	Y = 0b010,
	Z = 0b100
};

static const int default_sqaure_vals = 54;
static const float default_square[default_sqaure_vals] = {
    0.5f, 0.5f, 0.f,   1.f, 0.f, 0.f,   0.f, 0.f, 0.f,
   -0.5f, 0.5f, 0.f,   0.f, 1.f, 0.f,   0.f, 0.f, 0.f,
   -0.5f,-0.5f, 0.f,   0.f, 0.f, 1.f,   0.f, 0.f, 0.f,

   -0.5f,-0.5f, 0.f,   0.f, 0.f, 1.f,   0.f, 0.f, 0.f,
    0.5f,-0.5f, 0.f,   0.f, 1.f, 0.f,   0.f, 0.f, 0.f,
    0.5f, 0.5f, 0.f,   1.f, 0.f, 0.f,   0.f, 0.f, 0.f
};

class Rect : public IDrawable
{
private:
	struct axis_type
	{
		axis_type(Axis_t axis)
		{
			x = ~(axis % 2);
			y = ~((axis >> 1) % 2);
			z = ~((axis >> 2) % 2);
		}
		
		unsigned int x : 1;
		unsigned int y : 1;
		unsigned int z : 1;
	};
public:

	Rect() = default;

	void initialise(glm::mat4& projection) override
	{ 
		//left = positive-x
		//up = positive-y
		//back = positive-z

		//1.0 parts wide
		setVertices(default_square, default_sqaure_vals);

		setShader(
			"Data/shaders/vertex/vertex_shader.txt",
			"Data/shaders/fragment/fragment_shader.txt",
			projection);
	}

	void initialise(glm::mat4& projection, Shader* shader) override
	{
		//left = positive-x
		//up = positive-y
		//back = positive-z

		//1.0 parts wide
		setVertices(default_square, default_sqaure_vals);

		setShader(shader,
			projection);
	}

	void setFacing(Axis_t facing_axis, bool reverse_winding = false)
	{
		axis_type axis = facing_axis;	
		m_current_axis = facing_axis;

		if(reverse_winding)
		{
			setVertices({
						0.5f * axis.x,   0.5f * axis.y,  (axis.x ? 0.5f : -0.5f) * axis.z,   1.f, 0.f, 0.f,   0.f, 0.f, 0.f,
					 -0.5f * axis.x,   0.5f * axis.y,  0.5f * axis.z                   ,   0.f, 1.f, 0.f,   0.f, 0.f, 0.f,
					 -0.5f * axis.x,  -0.5f * axis.y,  (axis.x ? -0.5f : 0.5f) * axis.z,   0.f, 0.f, 1.f,   0.f, 0.f, 0.f,

					 -0.5f * axis.x,  -0.5f * axis.y,  (axis.x ? -0.5f : 0.5f) * axis.z,   0.f, 0.f, 1.f,   0.f, 0.f, 0.f,
						0.5f * axis.x,  -0.5f * axis.y, -0.5f * axis.z,                      0.f, 1.f, 0.f,   0.f, 0.f, 0.f,
						0.5f * axis.x,   0.5f * axis.y,  (axis.x ? 0.5f : -0.5f) * axis.z,   1.f, 0.f, 0.f,   0.f, 0.f, 0.f
				});
		}
		else
		{
			setVertices({
						0.5f * axis.x,   0.5f * axis.y,  (axis.x ? 0.5f : -0.5f) * axis.z,   1.f, 0.f, 0.f,   0.f, 0.f, 0.f,
						0.5f * axis.x,  -0.5f * axis.y, -0.5f * axis.z,                      0.f, 1.f, 0.f,   0.f, 0.f, 0.f,
					 -0.5f * axis.x,  -0.5f * axis.y,  (axis.x ? -0.5f : 0.5f) * axis.z,   0.f, 0.f, 1.f,   0.f, 0.f, 0.f,
					 -0.5f * axis.x,  -0.5f * axis.y,  (axis.x ? -0.5f : 0.5f) * axis.z,   0.f, 0.f, 1.f,   0.f, 0.f, 0.f,
					 -0.5f * axis.x,   0.5f * axis.y,  0.5f * axis.z                   ,   0.f, 1.f, 0.f,   0.f, 0.f, 0.f,
						0.5f * axis.x,   0.5f * axis.y,  (axis.x ? 0.5f : -0.5f) * axis.z,   1.f, 0.f, 0.f,   0.f, 0.f, 0.f,

				});
		}
	}

	Axis_t getAxis() const { return m_current_axis; }

private:
	Axis_t m_current_axis;
};