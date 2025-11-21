#pragma once

#include <array>

#include "Rect.h"

class Cube : public IDrawable
{
public:

	Cube()
	{
	}

	void initialise(glm::mat4& projection) override
	{
		for (int i = 0; i < 6; i++)
		{
			faces[i]->initialise(projection);
		}

		faces[0]->setPosition({ -0.5,0,0 });
		faces[0]->setFacing(Axis_t::X, true);

		faces[1]->setPosition({ 0.5,0,0 });
		faces[1]->setFacing(Axis_t::X);

		faces[2]->setPosition({ 0,-0.5,0 });
		faces[2]->setFacing(Axis_t::Y);

		faces[3]->setPosition({ 0,0.5,0 });
		faces[3]->setFacing(Axis_t::Y, true);

		faces[4]->setPosition({ 0,0,-0.5 });
		faces[4]->setFacing(Axis_t::Z, true);

		faces[5]->setPosition({ 0,0,0.5 });
		faces[5]->setFacing(Axis_t::Z);
	}

	void initialise(glm::mat4& projection, Shader& shader) override
	{
		for (int i = 0; i < 6; i++)
		{
			faces[i]->initialise(projection, shader);
		}

		faces[0]->setPosition({ -0.5,0,0 });
		faces[0]->setFacing(Axis_t::X, true);

		faces[1]->setPosition({ 0.5,0,0 });
		faces[1]->setFacing(Axis_t::X);

		faces[2]->setPosition({ 0,-0.5,0 });
		faces[2]->setFacing(Axis_t::Y);

		faces[3]->setPosition({ 0,0.5,0 });
		faces[3]->setFacing(Axis_t::Y, true);

		faces[4]->setPosition({ 0,0,-0.5 });
		faces[4]->setFacing(Axis_t::Z, true);

		faces[5]->setPosition({ 0,0,0.5 });
		faces[5]->setFacing(Axis_t::Z);
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

	std::array<Rect*, 6> getFaces() { return faces; }

private:

	void draw(unsigned int& VAO, unsigned int& VBO, glm::mat4& view, DrawWindow& window) override
	{
		for (int i = 0; i < 6; i++)
		{
			window.draw(*faces[i]);
		}
	}

private:

	std::array<Rect*, 6> faces = {new Rect(), new Rect(), new Rect(), new Rect(), new Rect(), new Rect()};

};