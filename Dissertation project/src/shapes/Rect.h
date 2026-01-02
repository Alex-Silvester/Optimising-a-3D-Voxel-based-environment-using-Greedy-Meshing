#pragma once

#include "../Interfaces/IDrawable.h"

enum Axis_t
{
	EMPTY = 0,
	X = 0b001,
	Y = 0b010,
	Z = 0b100
};

//left = positive-x
//up = positive-y
//back = positive-z
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
		//1.0 parts wide
		setVertices(default_square, default_sqaure_vals);

		setShader(shader,
			projection);
	}

	void setFacing(Axis_t facing_axis, bool reverse_winding = false)
	{
		axis_type axis = facing_axis;	
		m_current_axis = facing_axis;

		glm::vec3 normal = facing_axis == X ? glm::vec3{1, 0, 0} : (facing_axis == Y ? glm::vec3{0, 1, 0} : glm::vec3{0, 0, 1});

		if(reverse_winding)
		{
			setVertices({
						0.5f * axis.x,   0.5f * axis.y,  (axis.x ? 0.5f : -0.5f) * axis.z,   1.f, 0.f, 0.f,   -normal.x, normal.y, -normal.z,
					 -0.5f * axis.x,   0.5f * axis.y,  0.5f * axis.z                   ,   0.f, 1.f, 0.f,   -normal.x, normal.y, -normal.z,
					 -0.5f * axis.x,  -0.5f * axis.y,  (axis.x ? -0.5f : 0.5f) * axis.z,   0.f, 0.f, 1.f,   -normal.x, normal.y, -normal.z,

					 -0.5f * axis.x,  -0.5f * axis.y,  (axis.x ? -0.5f : 0.5f) * axis.z,   0.f, 0.f, 1.f,   -normal.x, normal.y, -normal.z,
						0.5f * axis.x,  -0.5f * axis.y, -0.5f * axis.z,                      0.f, 1.f, 0.f,   -normal.x, normal.y, -normal.z,
						0.5f * axis.x,   0.5f * axis.y,  (axis.x ? 0.5f : -0.5f) * axis.z,   1.f, 0.f, 0.f,   -normal.x, normal.y, -normal.z
				});
		}
		else
		{
			setVertices({
						0.5f * axis.x,   0.5f * axis.y,  (axis.x ? 0.5f : -0.5f) * axis.z,   1.f, 0.f, 0.f,   normal.x, -normal.y, normal.z,
						0.5f * axis.x,  -0.5f * axis.y, -0.5f * axis.z,                      0.f, 1.f, 0.f,   normal.x, -normal.y, normal.z,
					 -0.5f * axis.x,  -0.5f * axis.y,  (axis.x ? -0.5f : 0.5f)* axis.z,    0.f, 0.f, 1.f,   normal.x, -normal.y, normal.z,

					 -0.5f * axis.x,  -0.5f * axis.y,  (axis.x ? -0.5f : 0.5f)* axis.z,    0.f, 0.f, 1.f,   normal.x, -normal.y, normal.z,
					 -0.5f * axis.x,   0.5f * axis.y,  0.5f * axis.z                   ,   0.f, 1.f, 0.f,   normal.x, -normal.y, normal.z,
						0.5f * axis.x,   0.5f * axis.y,  (axis.x ? 0.5f : -0.5f)* axis.z,    1.f, 0.f, 0.f,   normal.x, -normal.y, normal.z
				});
		}
	}

	void setPosition(const glm::vec3& vec) override
	{
		m_rect_position = vec;
		IDrawable::setPosition(vec);
	}

	const glm::vec3& getPosition() const override
	{
		return m_rect_position;
	}

	bool scaleX(float scale)
	{
		//don't attempt to scale the axis direction as it changes the position of the rect
		if (m_current_axis == Axis_t::X) return false;

		this->scale({ scale, 1, 1 });
		return true;
	}

	bool scaleY(float scale)
	{
		//don't attempt to scale the axis direction as it changes the position of the rect
		if (m_current_axis == Axis_t::Y) return false;

		this->scale({ 1, scale, 1 });
		return true;
	}

	bool scaleZ(float scale)
	{
		//don't attempt to scale the axis direction as it changes the position of the rect
		if (m_current_axis == Axis_t::Z) return false;

		this->scale({ 1, 1, scale });
		return true;
	}

	void scaleAndMoveX(float scale)
	{
		this->scale({scale,1,1}, { 0.5f,0,0 });
	}

	void scaleAndMoveY(float scale)
	{
		this->scale({ 1,scale,1 }, { 0,0.5f,0 });
	}

	void scaleAndMoveZ(float scale)
	{
		this->scale({ 1,1,scale }, { 0,0,0.5f });
	}

	Axis_t getAxis() const { return m_current_axis; }

private:
	Axis_t m_current_axis = Axis_t::EMPTY;
	glm::vec3 m_rect_position = {0,0,0};
};