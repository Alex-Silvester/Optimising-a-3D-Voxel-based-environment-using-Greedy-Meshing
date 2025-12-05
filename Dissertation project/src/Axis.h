#pragma once

#include <print>
#include <algorithm>
#include <ranges>
#include <vector>
#include <numeric>

#include "Interfaces/IDrawable.h"

#include "shapes/Cube.h"

#define FACE_SORT false
#define FACE_CULL true


class Axis : public IDrawable
{
public:

	explicit Axis(Axis_t axis, int axis_size, int axis_area) : m_axis(axis), axis_size(axis_size), axis_area(axis_area) {};

	~Axis() 
	{
		//for (Rect* face : faces)
		//{
		//	delete face;
		//}
	}

	void initialise(glm::mat4& projection) override {}
	void initialise(glm::mat4& projection, Shader* shader) override {}

	void addFaces(std::vector<Cube>& voxels)
	{
		using namespace std::ranges::views;

		std::vector<Rect*> temp_faces;
		for (Cube& voxel : voxels)
		{
			temp_faces.append_range(voxel.getFaces() | filter([this](Rect* face) {return face->getAxis() == m_axis; }));
		}

#if FACE_SORT
		std::sort(faces.begin(), faces.end(), [this](Rect* face_a, Rect* face_b) {return faceSorter(face_a, face_b); });
#endif

#if FACE_CULL

		//essentially making copy_if into move_if
		std::copy_if(
			std::make_move_iterator(temp_faces.begin()),
			std::make_move_iterator(temp_faces.end()),
			std::back_inserter(faces), 
			[this, &temp_faces](Rect* face) {return !isFaceCovered(face, temp_faces); });

#endif
	}

private:

	float axis_pos(const Rect* face) const
	{
		return m_axis == X ? face->getPosition().x : (m_axis == Y ? face->getPosition().y : face->getPosition().z);
	}

	bool faceSorter(const Rect* face_a, const Rect* face_b) const
	{
		float pos_a = axis_pos(face_a);
		float pos_b = axis_pos(face_b);

		return pos_a < pos_b;
	}

	//returns true if the face is covered and shouldn't be shown
	bool isFaceCovered(const Rect* face, const std::vector<Rect*>& other_faces) const
	{
		//if the face is on an edge, return early
		float axis_check = axis_pos(face);
		if (axis_check < 0 || axis_check > axis_size)
		{
			return false;
		}

		glm::vec3 pos = face->getPosition();

		//chec if the face os covered by another face
		for (int idx = 0; idx < other_faces.size(); idx++)
		{
			//if the current face is the same as the input face
			if (face == other_faces[idx])
			{
				continue;
			}
			
			//if the current face's position is the same as the input face's position
			if (pos == other_faces[idx]->getPosition())
			{
				return true;
			}
		}
		return false;
	}

	void draw(unsigned int& VAO, unsigned int& VBO, glm::mat4& view, DrawWindow& window) override
	{
		for (Rect* face : faces)
		{
			if (face == nullptr) continue;
			window.draw(*face);
		}
	}

private:

	Axis_t m_axis = EMPTY;
	int axis_size;
	int axis_area;
	std::vector<Rect*> faces;
};