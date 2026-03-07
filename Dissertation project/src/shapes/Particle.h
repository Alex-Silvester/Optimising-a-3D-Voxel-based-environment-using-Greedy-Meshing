#pragma once

#include "../Interfaces/IDrawable.h"

template<Drawable T>
class Particle : public IDrawable
{
public:

	Particle()
	{
		m_shape.setAlwaysRendered(true);
	}

	Particle(glm::mat4 &projection)
	{
		m_shape.setAlwaysRendered(true);
		m_shape.initialise(projection);
	}

	Particle(glm::mat4 &projection, Shader *shader)
	{
		m_shape.setAlwaysRendered(true);
		m_shape.initialise(projection, shader);
	}

	void initialise(glm::mat4 &projection) override 
	{
		m_shape.initialise(projection); 
	}

	void initialise(glm::mat4 &projection, Shader *shader) override
	{
		m_shape.initialise(projection, shader);
	}

	T *operator->()
	{
		return &m_shape;
	}

	void scale(const glm::vec3 &scale)
	{
		m_shape.scale(scale * INV_SQRT_TWO);
	}

private:

	void draw(unsigned int &VAO, unsigned int &VBO, glm::mat4 &view, DrawWindow &window, unsigned int draw_mode = GL_TRIANGLES) override
	{
		glDisable(GL_CULL_FACE);

		window.draw(m_shape);

		glEnable(GL_CULL_FACE);
	}

private:

	T m_shape;

};