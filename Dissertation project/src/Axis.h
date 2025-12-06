#pragma once

#include <array>
#include <print>
#include <algorithm>
#include <ranges>
#include <vector>
#include <numeric>

#include "Interfaces/IDrawable.h"

#include "shapes/Cube.h"

#define FACE_SORT false
#define FACE_CULL true

#define SEARCH_THREADS 20

namespace std
{
	template< class InputIt, class OutputIt, class UnaryPred >
	inline void move_if(InputIt first, InputIt last, OutputIt d_first, UnaryPred pred)
	{
		std::copy_if(std::make_move_iterator(first), std::make_move_iterator(last), d_first, pred);
	}
}

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

	void addFaces(const std::vector<Cube>& voxels)
	{
		std::vector<Rect*> temp_faces;
		for (const Cube& voxel : voxels)
		{
			std::move_if(voxel.getFaces().begin(), voxel.getFaces().end(), std::back_inserter(temp_faces),
				[this](Rect* face) {return face->getAxis() == m_axis; });
		}

#if FACE_SORT
		std::sort(faces.begin(), faces.end(), [this](Rect* face_a, Rect* face_b) {return faceSorter(face_a, face_b); });
#endif

#if FACE_CULL

		for (int i = 0; i < SEARCH_THREADS; i++)
		{
			thread_pool[i] = std::thread([this, i, temp_faces] { checkFaces(i, temp_faces, temp_vectors[i]); });
		}

		for (int i = 0; i < SEARCH_THREADS; i++)
		{
			thread_pool[i].join();
		}

		for (auto& vec : temp_vectors)
		{
			faces.insert(faces.end(), vec.begin(), vec.end());
		}


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

	void checkFaces(const int i, const std::vector<Rect*>& temp_faces, std::vector<Rect*>& new_vec)
	{
		int start = temp_faces.size() * (float)i / SEARCH_THREADS;
		int end = temp_faces.size() * (float)(i + 1) / SEARCH_THREADS;

		std::move_if(temp_faces.begin() + start, temp_faces.begin() + end, std::back_inserter(new_vec),
			[this, &temp_faces](Rect* face) {return !isFaceCovered(face, temp_faces); });
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
		for (Rect* other_face : other_faces)
		{
			//if the current face is the same as the input face
			if (face == other_face)
			{
				continue;
			}

			const glm::vec3& other_face_pos = other_face->getPosition();


			//using early returns to speed up comparing two vectors
			// comparing two glm::vec3-s is slower than comparing two floats
			if (other_face_pos.x != pos.x)
			{
				continue;
			}

			if (other_face_pos.y != pos.y)
			{
				continue;
			}

			if (other_face_pos.z != pos.z)
			{
				continue;
			}

			return true;
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

	std::array<std::thread, SEARCH_THREADS> thread_pool;
	std::array<std::vector<Rect*>, SEARCH_THREADS> temp_vectors;
};