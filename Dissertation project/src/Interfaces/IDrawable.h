#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <print>

#include "../Shader Types/CubeShader.h"
#include "../Window/DrawWindow.h"

#include "../Helpers/Settings.h"

class IDrawable
{
public:

  //default initialisation for the drawable object
  virtual void initialise(glm::mat4& projection) = 0;
  virtual void initialise(glm::mat4& projection, Shader* shader) = 0;

  void setShader(const char* vertex_path, const char* fragment_path, glm::mat4& projection)
  {
    m_shader->init(vertex_path, fragment_path);

    m_shader->use();
    m_shader->setVec3("objectColor", 1.f,0.f,0.f);
    m_shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    m_shader->setVec3("lightPos", glm::vec3(0,0,0));
    m_shader->setInt("intensity", 1);

    // pass projection matrix to shader (note that in this case it could change every frame)
    m_shader->setMat4("projection", projection);
  }

  void setShader(Shader* shader, glm::mat4& projection)
  {
    m_shader = shader;

    m_shader->use();
    m_shader->setVec3("objectColor", 1.0f, 1.0f, 1.0f);
    m_shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    m_shader->setVec3("lightPos", glm::vec3(0, 0, 0));
    m_shader->setInt("intensity", 1);

    // pass projection matrix to shader (note that in this case it could change every frame)
    m_shader->setMat4("projection", projection);
  }

  void setShader(Shader* shader)
  {
    m_shader = (Shader*)(shader);

    m_shader->use();
    m_shader->setVec3("objectColor", 1.0f, 1.0f, 1.0f);
    m_shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    m_shader->setVec3("lightPos", glm::vec3(0, 0, 0));
    m_shader->setInt("intensity", 1);
  }

  void setProjection(glm::mat4 &projection)
  {
    m_shader->setMat4("projection", projection);
  }
  
  void setVertices(const std::vector<float>& vertices)
  {
    m_vertices = vertices;
  }

  void setVertices(const float* vertices, int vals)
  {
    this->m_vertices = std::vector<float>(vertices, vertices + vals);
  }

  Shader& getShader()
  {
    return *m_shader;
  }

  const std::vector<float>& getVertices() const
  {
    return m_vertices;
	}

  std::vector<float> getVerticesWithPosition() const
  {
    std::vector<float> global_vertices;
    for (int i = 0; i < m_vertices.size(); i++)
    {
      if (i % 9 == 0)
      {
        global_vertices.emplace_back(m_vertices[i] + m_position.x);
      }
      else if (i % 9 == 1)
      {
        global_vertices.emplace_back(m_vertices[i] + m_position.y);
      }
      else if (i % 9 == 2)
      {
        global_vertices.emplace_back(m_vertices[i] + m_position.z);
      }
      else
      {
        global_vertices.emplace_back(m_vertices[i]);
      }
    }

    return global_vertices;
  }

  virtual void setPosition(const glm::vec3& pos)
  {
    m_position = pos;
  }

  void scale(glm::vec3 scale, glm::vec3 offset = {0,0,0})
  {
    m_scale *= scale;

    for (int i = 0; i < m_vertices.size(); i++)
    {
      if (i % 9 <= 2)
      {
        if (i % 3 == 0)
        {
          m_vertices[i] += offset.x;
          m_vertices[i] *= scale.x;
          m_vertices[i] -= offset.x;
        }
        else if (i % 3 == 1)
        {
          m_vertices[i] += offset.y;
          m_vertices[i] *= scale.y;
          m_vertices[i] -= offset.y;
        }
        else
        {
          m_vertices[i] += offset.z;
          m_vertices[i] *= scale.z;
          m_vertices[i] -= offset.z;
        }
      }
    }
  }

  virtual const glm::vec3& getPosition() const
  {
    return m_position;
  }

  glm::vec3 getCenter()
  {
    glm::vec3 return_val;
    int acc = 0;
    for (int i = 0; i < m_vertices.size(); i++)
    {
      if (i % 9 == 2)
      {
        return_val += glm::vec3{m_vertices[i - 2], m_vertices[i - 1], m_vertices[i]};
        acc++;
      }
    }

    return_val /= acc;

    return return_val;
  }

  void setLayer(int new_layer)
  {
    layer = new_layer;
  }

  int getLayer()
  {
    return layer;
  }

  const glm::vec3 &getScale() const { return m_scale; }

  void setColor(const glm::vec3 &col)
  {
    m_color = col;
  }

private:

  friend class DrawWindow;
	
	virtual void draw(unsigned int& VAO, unsigned int& VBO, glm::mat4& view, DrawWindow& window)
  {
    m_shader->use();
    m_shader->setMat4("view", view);
    m_shader->setVec3("position", m_position);
    m_shader->setVec3("scale", m_scale);
    m_shader->setVec3("objectColor", m_color);

    // render
    glBindVertexArray(VAO);

    //explicitly bind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), &m_vertices[0], GL_STATIC_DRAW);

    // calculate the model matrix for each object and pass it to shader before drawing
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    model = glm::translate(model, glm::vec3(0.f));
    m_shader->setMat4("model", model);

  #if OCCLUSION_CULL_QUERY == true
    glGenQueries(1, &occ_query);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    glBeginQuery(GL_ANY_SAMPLES_PASSED, occ_query);

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 9);

    glEndQuery(GL_ANY_SAMPLES_PASSED);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    glGetQueryObjectiv(occ_query, GL_QUERY_RESULT, &passed);
    

    if (prev_passed)
    {
      glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 9);
      prev_passed = (passed != 0);
    }
    else
    {
      prev_passed = (passed != 0);
      return;
    }
  #else
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 9);
  #endif
	}
  
protected:

  glm::vec3 m_position = { 0,0,0 };
  glm::vec3 m_scale = { 1.0f , 1.0f, 1.0f };
	std::vector<float> m_vertices;

private:

  Shader* m_shader = nullptr;

  glm::vec3 m_color = { 0.f,1.f,0.f };

#if OCCLUSION_CULL_QUERY == true
  GLuint occ_query;
  int passed = 0;
#endif

  bool prev_passed = true;
  int layer = 0;
};