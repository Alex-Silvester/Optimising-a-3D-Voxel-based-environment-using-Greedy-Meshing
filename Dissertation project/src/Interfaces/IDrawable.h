#pragma once

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "../shader.h"

class IDrawable
{
public:

  void setShader(const char* vertex_path, const char* fragment_path)
  {
    m_shader.init(vertex_path, fragment_path);
  }

  void setVertices(const std::vector<float>& vertices)
  {
    this->vertices = vertices;
  }

private:

	friend class DrawWindow;
	
	void draw(unsigned int VAO, glm::mat4& view)
  {
    m_shader.setMat4("view", view);

    // render
    glBindVertexArray(VAO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // calculate the model matrix for each object and pass it to shader before drawing
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    model = glm::translate(model, glm::vec3(0.f));
    m_shader.setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 8);
	}

private:

	std::vector<float> vertices;
  Shader m_shader;
};