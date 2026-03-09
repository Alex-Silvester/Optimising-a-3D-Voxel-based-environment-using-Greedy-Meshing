#pragma once

#include "../Interfaces/IDrawable.h"

#include <cmath>
#include <glm/fwd.hpp>

#include "../Shader Types/shader.h"
#include "../Helpers/HelperFunctions.h"

#include "Frustum.h"

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
    0.5f, 0.5f, 0.f,   1.f, 1.f, 1.f,   0.f, 0.f, 0.f,
   -0.5f, 0.5f, 0.f,   1.f, 1.f, 1.f,   0.f, 0.f, 0.f,
   -0.5f,-0.5f, 0.f,   1.f, 1.f, 1.f,   0.f, 0.f, 0.f,

   -0.5f,-0.5f, 0.f,   1.f, 1.f, 1.f,   0.f, 0.f, 0.f,
    0.5f,-0.5f, 0.f,   1.f, 1.f, 1.f,   0.f, 0.f, 0.f,
    0.5f, 0.5f, 0.f,   1.f, 1.f, 1.f,   0.f, 0.f, 0.f
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

		update_corner_positions = true;
	}

	void initialise(glm::mat4& projection, Shader* shader) override
	{
		//1.0 parts wide
		setVertices(default_square, default_sqaure_vals);

		setShader(shader, projection);

		update_corner_positions = true;
	}

	void setFacing(Axis_t facing_axis, bool reverse_winding = false)
	{
		axis_type axis = facing_axis;	
		m_current_axis = facing_axis;

		glm::vec3 normal = facing_axis == X ? glm::vec3{1, 0, 0} : (facing_axis == Y ? glm::vec3{0, 1, 0} : glm::vec3{0, 0, 1});

		if(reverse_winding)
		{
			setVertices({
						0.5f * axis.x,   0.5f * axis.y,  (axis.x ? 0.5f : -0.5f) * axis.z,   1.f, 1.f, 1.f,   -normal.x, normal.y, -normal.z,
					 -0.5f * axis.x,   0.5f * axis.y,  0.5f * axis.z                   ,   1.f, 1.f, 1.f,   -normal.x, normal.y, -normal.z,
					 -0.5f * axis.x,  -0.5f * axis.y,  (axis.x ? -0.5f : 0.5f) * axis.z,   1.f, 1.f, 1.f,   -normal.x, normal.y, -normal.z,

					 -0.5f * axis.x,  -0.5f * axis.y,  (axis.x ? -0.5f : 0.5f) * axis.z,   1.f, 1.f, 1.f,   -normal.x, normal.y, -normal.z,
						0.5f * axis.x,  -0.5f * axis.y, -0.5f * axis.z,                      1.f, 1.f, 1.f,   -normal.x, normal.y, -normal.z,
						0.5f * axis.x,   0.5f * axis.y,  (axis.x ? 0.5f : -0.5f) * axis.z,   1.f, 1.f, 1.f,   -normal.x, normal.y, -normal.z
				});
		}
		else
		{
			setVertices({
						0.5f * axis.x,   0.5f * axis.y,  (axis.x ? 0.5f : -0.5f) * axis.z,   1.f, 1.f, 1.f,   normal.x, -normal.y, normal.z,
						0.5f * axis.x,  -0.5f * axis.y, -0.5f * axis.z,                      1.f, 1.f, 1.f,   normal.x, -normal.y, normal.z,
					 -0.5f * axis.x,  -0.5f * axis.y,  (axis.x ? -0.5f : 0.5f)* axis.z,    1.f, 1.f, 1.f,   normal.x, -normal.y, normal.z,

					 -0.5f * axis.x,  -0.5f * axis.y,  (axis.x ? -0.5f : 0.5f)* axis.z,    1.f, 1.f, 1.f,   normal.x, -normal.y, normal.z,
					 -0.5f * axis.x,   0.5f * axis.y,  0.5f * axis.z                   ,   1.f, 1.f, 1.f,   normal.x, -normal.y, normal.z,
						0.5f * axis.x,   0.5f * axis.y,  (axis.x ? 0.5f : -0.5f)* axis.z,    1.f, 1.f, 1.f,   normal.x, -normal.y, normal.z
				});
		}

		update_corner_positions = true;
	}

	void setPosition(const glm::vec3& vec) override
	{
		IDrawable::setPosition(vec);

		update_corner_positions = true;
	}

	bool scaleX(float scale)
	{
		//don't attempt to scale the axis direction as it changes the position of the rect
		if (m_current_axis == Axis_t::X) return false;

		this->scale({ scale, 1, 1 });

		update_corner_positions = true;

		return true;
	}

	bool scaleY(float scale)
	{
		//don't attempt to scale the axis direction as it changes the position of the rect
		if (m_current_axis == Axis_t::Y) return false;

		this->scale({ 1, scale, 1 });

		update_corner_positions = true;

		return true;
	}

	bool scaleZ(float scale)
	{
		//don't attempt to scale the axis direction as it changes the position of the rect
		if (m_current_axis == Axis_t::Z) return false;

		this->scale({ 1, 1, scale });

		update_corner_positions = true;

		return true;
	}

	template<Axis_t axis>
	void scaleAndMove(float scale) {}

	template<>
	void scaleAndMove<Axis_t::X>(float scale)
	{
		this->scale({scale,1,1}, { 0.5f,0,0 });

		update_corner_positions = true;
	}

	template<>
	void scaleAndMove<Axis_t::Y>(float scale)
	{
		this->scale({ 1,scale,1 }, { 0,0.5f,0 });

		update_corner_positions = true;
	}

	template<>
	void scaleAndMove<Axis_t::Z>(float scale)
	{
		this->scale({ 1,1,scale }, { 0,0,0.5f });

		update_corner_positions = true;
	}

	/// <summary>
	/// Merges the current face and the given face given they meet the correct requirements.
	/// A naive implementation assuming the use case is what is being done is the GM algorithm, 
  /// not generalized for other use
	/// </summary>
	/// <param name="other_rect">The rect attempting to be merged with</param>
	/// <returns></returns>
	template<Axis_t check_axis, Axis_t merge_axis>
	bool mergeRects(const Rect &other_rect)
	{
		//if the rects aren't on the same axis then don't attempt to merge them
		if (other_rect.getAxis() != m_current_axis) return false;

		//if the rects aren't on the same plane, then dont attempt to merge them
		if (other_rect.axisPos() != this->axisPos()) return false;

		if (other_rect.axisPos(check_axis) != this->axisPos(check_axis)) return false;

		//if the axis that is being merged into doesn't have the same scale 
		// (i.e. merging in the x axis along z requires the y scale to be the same), return false
		if (other_rect.axisScale(check_axis) != this->axisScale(check_axis)) return false;

		if (this->axisPos(merge_axis) + this->axisScale(merge_axis)!= other_rect.axisPos(merge_axis)) return false;

		float new_scale = this->axisScale(merge_axis) + other_rect.axisScale(merge_axis);

		scaleAndMove<merge_axis>(1.f / this->axisScale(merge_axis));

		scaleAndMove<merge_axis>(new_scale);

		return true;
	}

	Axis_t getAxis() const { return m_current_axis; }

	float axisPos(Axis_t axis = Axis_t::EMPTY) const
	{
		if (axis == Axis_t::EMPTY) axis = m_current_axis;

		switch (axis)
		{
			case Axis_t::X: return m_position.x;
			case Axis_t::Y: return m_position.y;
			case Axis_t::Z: return m_position.z;
		}
		return NAN;
	}

	float axisScale(Axis_t axis = Axis_t::EMPTY) const
	{
		if (axis == Axis_t::EMPTY) axis = m_current_axis;

		switch (axis)
		{
			case Axis_t::X: return m_scale.x;
			case Axis_t::Y: return m_scale.y;
			case Axis_t::Z: return m_scale.z;
		}
		return NAN;
	}

	const std::array<glm::vec3, 4>& getCorners()
	{
		if (update_corner_positions)
		{
			updateCorners();
		}

		return corner_positions;
	}

	void updateCorners()
	{
		corner_positions = std::array<glm::vec3, 4>
		{
			glm::vec3(m_vertices[0], m_vertices[1], m_vertices[2]) + m_position,
			glm::vec3(m_vertices[9], m_vertices[10], m_vertices[11]) + m_position,
			glm::vec3(m_vertices[18], m_vertices[19], m_vertices[20]) + m_position,
			glm::vec3(m_vertices[36], m_vertices[37], m_vertices[38]) + m_position,
		};
	}

#if FRUSTUM_CULLING == true
	void testFrustum(const Frustum &frustum)
	{
		updateCorners();
		setFrustumPass(false);

		if (
			isOnOrForwardPlane(frustum.getPlane(0)) &&
			isOnOrForwardPlane(frustum.getPlane(1)) &&
			isOnOrForwardPlane(frustum.getPlane(2)) &&
			isOnOrForwardPlane(frustum.getPlane(3)) &&
			isOnOrForwardPlane(frustum.getPlane(4)) &&
			isOnOrForwardPlane(frustum.getPlane(5)))
		{
			setFrustumPass(true);
		}
	}

	bool inFront(const glm::vec3 &point, const glm::vec3 &direction, float dist)
	{
		const glm::vec3 &q = point;
		const glm::vec3 &p = point + direction * dist;

		std::array<glm::vec3, 4> corners = getCorners();
		const glm::vec3 &a = corners[0];
		const glm::vec3 &b = corners[1];
		const glm::vec3 &c = corners[2];
		const glm::vec3 &d = corners[3];


		glm::vec3 pq = q - p;
		glm::vec3 pa = a - p;
		glm::vec3 pb = b - p;
		glm::vec3 pc = c - p;

		glm::vec3 m = glm::cross(pc, pq);
		float v = glm::dot(pa, m);
		if (v >= 0.0f)
		{
			float u = -glm::dot(pb, m);
			if (u < 0.0f) return false;
			float w = hf::scalarTriple(pq, pb, pa);
			if (w < 0.0f) return false;
		}
		else
		{
			glm::vec3 pd = d - p;

			float u = glm::dot(pd, m);
			if (u < 0.0f) return false;
			float w = hf::scalarTriple(pq, pa, pd);
			if (w < 0.0f) return false;
		}

		return true;
	}

	glm::vec3 closestPoint(const glm::vec3 &p)
	{
		updateCorners();

		const glm::vec3 &a = corner_positions[0];
		const glm::vec3 &b = corner_positions[1];
		const glm::vec3 &c = corner_positions[3];

		using Vector = glm::vec3;

		Vector ab = b - a;
		Vector ac = c - a;
		Vector d = p - a;

		Vector q = a;

		float dist = glm::dot(d, ab);
		float maxDist = glm::dot(ab, ab);
		if (dist >= maxDist)
			q += ab;
		else if (dist > 0.0f)
			q += (dist / maxDist) * ab;

		dist = glm::dot(d, ac);
		maxDist = glm::dot(ac, ac);
		if (dist >= maxDist)
			q += ac;
		else if (dist > 0.0f)
			q += (dist / maxDist) * ac;

		return q;
	}

	bool isOnOrForwardPlane(const Plane &plane)
	{
		int in_front = 0;

		for (glm::vec3 p : corner_positions)
		{
			if (glm::dot(plane.m_normal, p-plane.m_position) > 0)
			{
				in_front++;
			}
		}

		return in_front > 0;
	}

#endif

private:
	Axis_t m_current_axis = Axis_t::EMPTY;

	bool update_corner_positions = false;
	std::array <glm::vec3, 4> corner_positions;
};