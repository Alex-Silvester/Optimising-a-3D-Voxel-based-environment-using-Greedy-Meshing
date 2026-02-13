#pragma once

#include "../Interfaces/IDrawable.h"

template<Drawable T>
class WireFrame : public IDrawable
{
public:

	void initialise(glm::mat4 &projection) override { m_shape.initialise(projection); }
	void initialise(glm::mat4 &projection, Shader *shader) override { m_shape.initialise(projection, shader); }

	T &getShape()
	{
		return m_shape;
	}

	T* operator->()
	{
		return &m_shape;
	}

private:

	void draw(unsigned int &VAO, unsigned int &VBO, glm::mat4 &view, DrawWindow &window, unsigned int draw_mode = GL_TRIANGLES) override
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
		
		window.draw(m_shape);

		glEnable(GL_CULL_FACE);

		if(!window.isWireFrame())
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

private:

	T m_shape;

};