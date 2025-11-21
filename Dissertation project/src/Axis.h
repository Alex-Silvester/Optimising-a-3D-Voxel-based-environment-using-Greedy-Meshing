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

	explicit Axis(Axis_t axis) : m_axis(axis) {};

	~Axis() 
	{
		//for (Rect* face : faces)
		//{
		//	delete face;
		//}
	}

	void initialise(glm::mat4& projection) override {}
	void initialise(glm::mat4& projection, Shader& shader) override {}

	void addFaces(std::vector<Rect*> faces)
	{
		std::copy(faces.begin(), faces.end(), std::back_inserter(this->faces));
	}

	void addFaces(std::vector<Cube>& voxels)
	{
		using namespace std::ranges::views;
		for (Cube& voxel : voxels)
		{
			faces.append_range(voxel.getFaces() | filter([this](Rect* face) {return face->getAxis() == m_axis; }));
		}
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
	std::vector<Rect*> faces;
};