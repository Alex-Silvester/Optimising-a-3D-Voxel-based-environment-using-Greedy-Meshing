#pragma once

#include <array>

#include "Rect.h"

class Cube : public IDrawable
{
public:

	Cube()
	{
	}

	Cube(glm::mat4& projection, const glm::vec3& position, Shader* shader)
	{
		initialiseFaces(projection, shader);
		setPosition(position);
	}

	Cube(glm::mat4& projection, const glm::vec3& position)
	{
		initialiseFaces(projection);
		setPosition(position);
	}

	void initialise(glm::mat4& projection) override
	{
		initialiseFaces(projection);
	}

	void initialise(glm::mat4& projection, Shader* shader) override
	{
		initialiseFaces(projection, shader);
	}

	void setPosition(const glm::vec3& pos) override
	{
		faces[0]->setPosition(glm::vec3(-0.5, 0, 0 ) + pos);
		faces[1]->setPosition(glm::vec3( 0.5, 0, 0 ) + pos);
		faces[2]->setPosition(glm::vec3( 0,-0.5, 0 ) + pos);
		faces[3]->setPosition(glm::vec3( 0, 0.5, 0 ) + pos);
		faces[4]->setPosition(glm::vec3( 0, 0,-0.5 ) + pos);
		faces[5]->setPosition(glm::vec3( 0, 0, 0.5 ) + pos);
	}

	const std::array<Rect*, 6>& getFaces() const { return faces; }

private:

	void draw(unsigned int& VAO, unsigned int& VBO, glm::mat4& view, DrawWindow& window) override
	{
		for (int i = 0; i < 6; i++)
		{
			window.draw(*faces[i]);
		}
	}

	void initialiseFaces(glm::mat4& projection, Shader* shader)
	{
		faces[0]->initialise(projection, shader);
		faces[1]->initialise(projection, shader);
		faces[2]->initialise(projection, shader);
		faces[3]->initialise(projection, shader);
		faces[4]->initialise(projection, shader);
		faces[5]->initialise(projection, shader);

		setPosition({ 0,0,0 });

		faces[1]->setFacing(Axis_t::X);
		faces[2]->setFacing(Axis_t::Y);
		faces[5]->setFacing(Axis_t::Z);

		faces[0]->setFacing(Axis_t::X, true);
		faces[3]->setFacing(Axis_t::Y, true);
		faces[4]->setFacing(Axis_t::Z, true);

		faces[1]->setLayer(1);
		faces[3]->setLayer(1);
		faces[5]->setLayer(1);

	}

	void initialiseFaces(glm::mat4& projection)
	{
		faces[0]->initialise(projection);
		faces[1]->initialise(projection);
		faces[2]->initialise(projection);
		faces[3]->initialise(projection);
		faces[4]->initialise(projection);
		faces[5]->initialise(projection);

		setPosition({ 0,0,0 });

		faces[1]->setFacing(Axis_t::X);
		faces[2]->setFacing(Axis_t::Y);
		faces[5]->setFacing(Axis_t::Z);

		faces[0]->setFacing(Axis_t::X, true);
		faces[3]->setFacing(Axis_t::Y, true);
		faces[4]->setFacing(Axis_t::Z, true);

		faces[1]->setLayer(1);
		faces[3]->setLayer(1);
		faces[5]->setLayer(1);
	}

private:

	std::array<Rect*, 6> faces = {new Rect(), new Rect(), new Rect(), new Rect(), new Rect(), new Rect()};

};