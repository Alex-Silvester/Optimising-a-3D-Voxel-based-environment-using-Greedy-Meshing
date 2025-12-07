#pragma once

#include <array>
#include <print>
#include <algorithm>
#include <ranges>
#include <vector>
#include <numeric>
#include <mutex>

#include "Interfaces/IDrawable.h"

#include "shapes/Cube.h"

#define FACE_SORT true
#define FACE_CULL true
#define USE_INSTANCING true

#define SEARCH_THREADS 10

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

	~Axis() {}

	void initialise(glm::mat4& projection) override {}
	void initialise(glm::mat4& projection, Shader* shader) override {}

	void addFaces(const std::vector<Cube>& voxels, std::mutex& mtx);

private:

	float axis_pos(const Rect* face) const
	{
		return m_axis == X ? face->getPosition().x : (m_axis == Y ? face->getPosition().y : face->getPosition().z);
	}

	bool faceSorter(const Rect* face_a, const Rect* face_b) const;

	void checkFaces(const int i, const std::vector<Rect*>& temp_faces, std::vector<Rect*>& new_vec);

	//returns true if the face is covered and shouldn't be shown
	bool isFaceCovered(const Rect* face, const std::vector<Rect*>& other_faces) const;

	void draw(unsigned int& VAO, unsigned int& VBO, glm::mat4& view, DrawWindow& window) override
	{
#if USE_INSTANCING == false
		for (Rect* face : faces)
		{
			if (face == nullptr) continue;
			window.draw(*face);
		}
#else
		window.draw(m_instanced_vertices, &faces[0]->getShader());
#endif
	}

private:

	Axis_t m_axis = EMPTY;
	int axis_size;
	int axis_area;
	std::vector<Rect*> faces;
	std::vector<glm::vec3> face_position_cache;

	std::array<std::thread, SEARCH_THREADS> thread_pool;
	std::array<std::vector<Rect*>, SEARCH_THREADS> temp_vectors;

	std::vector<float> m_instanced_vertices;
};

void Axis::addFaces(const std::vector<Cube>& voxels, std::mutex& mtx)
{
	std::vector<Rect*> temp_faces;
	for (const Cube& voxel : voxels)
	{
		std::move_if(voxel.getFaces().begin(), voxel.getFaces().end(), std::back_inserter(temp_faces),
			[this](Rect* face)
			{
				if (face->getAxis() == m_axis)
				{
					face_position_cache.emplace_back(face->getPosition());
					return true;
				}
				return false;
			});
	}

#if FACE_CULL

	for (int i = 0; i < SEARCH_THREADS; i++)
	{
		thread_pool[i] = std::move(std::thread([this, i, temp_faces] { checkFaces(i, temp_faces, temp_vectors[i]); }));
	}

	for (int i = 0; i < SEARCH_THREADS; i++)
	{
		thread_pool[i].join();
		faces.insert(faces.end(), std::make_move_iterator(temp_vectors[i].begin()), std::make_move_iterator(temp_vectors[i].end()));
	}

#if USE_INSTANCING
	for (auto& face : faces)
	{
		std::vector<float> global_vertices = face->getVerticesWithPosition();
		m_instanced_vertices.insert(m_instanced_vertices.end(), std::make_move_iterator(global_vertices.begin()), std::make_move_iterator(global_vertices.end()));
	}
#endif

#endif

#if FACE_SORT

	std::sort(faces.begin(), faces.end(), [this](Rect* face_a, Rect* face_b) {return faceSorter(face_a, face_b); });

#endif
}

inline bool Axis::faceSorter(const Rect* face_a, const Rect* face_b) const
{
	const glm::vec3& pos_a = face_a->getPosition();
	const glm::vec3& pos_b = face_b->getPosition();

	if (pos_a.x == pos_b.x)
	{
		if (pos_a.y == pos_b.y)
		{
			if (pos_a.z == pos_b.z)
			{
				return false;
			}
			else if (pos_a.z < pos_b.z)
			{
				return true;
			}
		}
		else if (pos_a.y < pos_b.y)
		{
			return true;
		}
	}
	else if (pos_a.x < pos_b.x)
	{
		return true;
	}

	return false;
}

void Axis::checkFaces(const int i, const std::vector<Rect*>& temp_faces, std::vector<Rect*>& new_vec)
{
	int start = temp_faces.size() * (float)i / SEARCH_THREADS;
	int end = temp_faces.size() * (float)(i + 1) / SEARCH_THREADS;

	std::move_if(temp_faces.begin() + start, temp_faces.begin() + end, std::back_inserter(new_vec),
		[this, &temp_faces](Rect* face) {return !isFaceCovered(face, temp_faces); });
}

bool Axis::isFaceCovered(const Rect* face, const std::vector<Rect*>& other_faces) const
{
	//if the face is on an edge, return early
	float axis_check = axis_pos(face);
	if (axis_check < 0 || axis_check > axis_size)
	{
		return false;
	}

	glm::vec3 pos = face->getPosition();

	//chec if the face os covered by another face
	for (int i = 0; i < other_faces.size(); i++)
	{
		//if the current face is the same as the input face
		if (face == other_faces[i])
		{
			continue;
		}

		const glm::vec3& other_pos = face_position_cache[i];

		//using early returns to speed up comparing two vectors
		// comparing two glm::vec3-s is slower than comparing two floats
		if (other_pos.x != pos.x)
		{
			continue;
		}

		if (other_pos.y != pos.y)
		{
			continue;
		}

		if (other_pos.z != pos.z)
		{
			continue;
		}

		return true;
	}

	return false;
}