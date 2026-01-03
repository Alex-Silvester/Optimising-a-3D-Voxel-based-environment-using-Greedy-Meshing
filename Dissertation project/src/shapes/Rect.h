#pragma once

#include "../Interfaces/IDrawable.h"
#include <cmath>
#include <glm/fwd.hpp>
#include "../Shader Types/shader.h"

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

/// <summary>
/// A default rect type that starts with a default 1x1x1 square to standardise 
/// the side lengths for merges and other functions
/// </summary>
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
		IDrawable::setPosition(vec);
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

	/// <summary>
	/// Merges the current face and the given face given they meet the correct requirements.
	/// A naive implementation assuming the use case is what is being done is the GM algorithm, 
  /// not generalized for other use
	/// </summary>
	/// <param name="other_rect">The rect attempting to be merged with</param>
	/// <returns></returns>
	bool mergeRects(const Rect &other_rect)
	{
		//if the rects aren't on the same axis then don't attempt to merge them
		if (other_rect.getAxis() != m_current_axis) return false;

		//if the rects aren't on the same plane, then dont attempt to merge them
		if (other_rect.axisPos() != this->axisPos()) return false;

		glm::vec3 direction = 
			glm::vec3(std::fabsf(other_rect.getPosition().x), std::fabsf(other_rect.getPosition().y), other_rect.getPosition().z) -
			glm::vec3(std::fabsf(m_position.x), std::fabsf(m_position.y), std::fabsf(m_position.z));

		if (direction.x != 0)
		{
			//if the faces arent the same size then don't attempt to merge them
			if (other_rect.getScale().y != m_scale.y) return false;

			float new_scale = m_scale.x + other_rect.getScale().x;

			scaleAndMoveX(new_scale/m_scale.x);

			return true;
		}

		if (direction.y != 0)
		{
			//if the faces arent the same size then don't attempt to merge them
			if (other_rect.getScale().x != m_scale.x) return false;

			float new_scale = m_scale.y + other_rect.getScale().y;

			scaleAndMoveX(new_scale / m_scale.y);

			return true;
		}

		if (direction.z != 0)
		{
			//if the faces arent the same size then don't attempt to merge them
			if (other_rect.getScale().y != m_scale.y) return false;

			float new_scale = m_scale.z + other_rect.getScale().z;

			scaleAndMoveZ(new_scale / m_scale.z);

			return true;
		}

		return false;
	}

	Axis_t getAxis() const { return m_current_axis; }

	float axisPos() const
	{
		switch (m_current_axis)
		{
			case Axis_t::X: return m_position.x;
			case Axis_t::Y: return m_position.y;
			case Axis_t::Z: return m_position.z;
		}
		return NAN;
	}

	float axisScale() const
	{
		switch (m_current_axis)
		{
			case Axis_t::X: return m_scale.x;
			case Axis_t::Y: return m_scale.y;
			case Axis_t::Z: return m_scale.z;
		}
		return NAN;
	}

private:
	Axis_t m_current_axis = Axis_t::EMPTY;
};