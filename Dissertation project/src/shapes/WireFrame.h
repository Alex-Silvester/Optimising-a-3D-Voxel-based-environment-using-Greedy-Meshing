#pragma once

#include "../Interfaces/IDrawable.h"

template<class T>
class WireFrame : public IDrawable
{
public:

	void initialise(glm::mat4 &projection) override {}
	void initialise(glm::mat4 &projection, Shader *shader) override {}

	T &getShape()
	{
		return m_shape;
	}

private:

	void draw(unsigned int &VAO, unsigned int &VBO, glm::mat4 &view, DrawWindow &window) override
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		window.draw(m_shape);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

private:

	T m_shape;

};