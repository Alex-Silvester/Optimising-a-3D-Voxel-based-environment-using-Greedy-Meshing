#pragma once

#include <print>
#include <algorithm>
#include <ranges>
#include <vector>
#include <numeric>

#include "Interfaces/IDrawable.h"

#include "shapes/Cube.h"

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
			faces.append_range(voxel.getFaces() | filter([this](Rect* face) {return face->getAxis() == m_axis; }));
		}

#if FACE_CULL
		std::sort(faces.begin(), faces.end(), [this](Rect* face_a, Rect* face_b) {return faceSorter(face_a, face_b); });

		int i = 0;
		faces.erase(std::remove_if(faces.begin(), faces.end(), [this,&i](Rect* face) {return removedCoveredFaces(face,i); }), faces.end());
		std::println("");
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
	bool removedCoveredFaces(const Rect* face, int& face_pos) const
	{
		glm::vec3 pos = face->getPosition();

		for (int idx = 1; idx < faces.size(); idx++)
		{
			if (face == faces[idx]) 
			{
				continue;
			}
			else if (pos == faces[idx]->getPosition())
			{
				face_pos++;
				return true;
			}
		}

		face_pos++;
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