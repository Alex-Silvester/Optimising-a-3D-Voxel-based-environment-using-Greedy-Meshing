#pragma once

#include <print>
#include <algorithm>
#include <ranges>
#include <vector>
#include <numeric>

#include "Interfaces/IDrawable.h"

#include "shapes/Cube.h"

#define FACE_CULL false


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
			faces.append_range(voxel.getFaces() | filter([this](Rect* face) {return face->getAxis() == m_axis; }));
		}

#if FACE_CULL
		std::sort(faces.begin(), faces.end(), [this](Rect* face_a, Rect* face_b) {return faceSorter(face_a, face_b); });

		faces.erase(std::remove_if(faces.begin(), faces.end(), [this](Rect* face) {return removedCoveredFaces(face); }), faces.end());
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
	bool removedCoveredFaces(const Rect* face) const
	{
		float pos = axis_pos(face);
		return pos > 0.f && pos < axis_size - 1;
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