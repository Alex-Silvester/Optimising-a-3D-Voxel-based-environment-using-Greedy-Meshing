#pragma once

#include <concepts>

#include "../Interfaces/IDrawable.h"

template<class T>
concept Drawable = requires(T t)
{
	dynamic_cast<IDrawable *>(&t);
};

template<Drawable T>
class WireFrame : public IDrawable
{
public:

	void initialise(glm::mat4 &projection) override {}
	void initialise(glm::mat4 &projection, Shader *shader) override {}

	T &getShape()
	{
		return m_shape;
	}

	T* operator->()
	{
		return &m_shape;
	}

private:

	void draw(unsigned int &VAO, unsigned int &VBO, glm::mat4 &view, DrawWindow &window) override
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