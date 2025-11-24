#pragma once

#include <print>
#include <algorithm>
#include <ranges>
#include <vector>
#include <numeric>

#include "Interfaces/IDrawable.h"

#include "shapes/Cube.h"


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
		for (Cube& voxel : voxels)
		{
			faces.append_range(voxel.getFaces() | filter([this](Rect* face) {return face->getAxis() == m_axis; }));
		}

		faces.erase(std::remove_if(faces.begin(), faces.end(), [this](Rect* face) {return removedCoveredFaces(face); }), faces.end());
	}

	bool removedCoveredFaces(Rect* face)
	{
		float pos = m_axis == X ? face->getPosition().x : (m_axis == Y ? face->getPosition().y : face->getPosition().z);

		return pos > 0.f && pos < axis_size - 1.f;
	}

private:

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