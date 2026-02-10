#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <print>

#include "../Shader Types/CubeShader.h"
#include "../Window/DrawWindow.h"

#include "../Helpers/Settings.h"
#include "../Helpers/Definitions.h"

class IDrawable;

template<class T>
concept Drawable = requires(T t)
{
  dynamic_cast<IDrawable *>(&t);
};

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
  }

  void setShader(Shader* shader, glm::mat4& projection)
  {
    m_shader = shader;

    m_shader->use();
  }

  void setShader(Shader* shader)
  {
    m_shader = (Shader*)(shader);

    m_shader->use();
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

  void setVertices(const std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> &vertices)
  {
    std::vector<float> new_vertices;
    for (auto& vert : vertices)
    {
        new_vertices.emplace_back(std::get<0>(vert).x);
        new_vertices.emplace_back(std::get<0>(vert).y);
        new_vertices.emplace_back(std::get<0>(vert).z);
        new_vertices.emplace_back(std::get<1>(vert).x);
        new_vertices.emplace_back(std::get<1>(vert).y);
        new_vertices.emplace_back(std::get<1>(vert).z);
        new_vertices.emplace_back(std::get<2>(vert).x);
        new_vertices.emplace_back(std::get<2>(vert).y);
        new_vertices.emplace_back(std::get<2>(vert).z);
    }
    setVertices(new_vertices);
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

  void setAlwaysRendered(bool _always_rendered)
  {
    always_rendered = _always_rendered;
  }

#if FRUSTUM_CULLING == true
  void setFrustumPass(bool pass)
  {
    passed_frustum = pass;
  }
#endif

  bool freecam_active = false;
  bool passed = false;
  int face_passed = true;

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
      drawVertices()
      prev_passed = (passed != 0);
    }
    else
    {
      prev_passed = (passed != 0);
      return;
    }
  #else

    drawVertices();

  #endif
	}

  void drawVertices()
  {
  #if FREECAM_ACTIVE == true

    if (always_rendered)
    {
      glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 9);
      return;
    }

    if (freecam_active && !passed)
    {
      passed = true;

      GLuint query;

      glGenQueries(1, &query);

      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthMask(GL_FALSE);

      glBeginQuery(GL_ANY_SAMPLES_PASSED, query);

      glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 9);

      glEndQuery(GL_ANY_SAMPLES_PASSED);

      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      glDepthMask(GL_TRUE);

      glGetQueryObjectiv(query, GL_QUERY_RESULT, &face_passed);
    }
    else if (!freecam_active)
    {
      face_passed = 1;
      passed = false;
    }

    if (face_passed != 0)
    {
      glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 9);
    }
  #else
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 9);
  #endif
  }
  
protected:

  glm::vec3 m_position = { 0,0,0 };
  glm::vec3 m_scale = { 1.0f , 1.0f, 1.0f };
	std::vector<float> m_vertices;

  bool always_rendered = false;

  Shader* m_shader = nullptr;

private:


  glm::vec3 m_color = { 0.f,1.f,0.f };

#if OCCLUSION_CULL_QUERY == true
  GLuint occ_query;
  int passed = 0;
#endif

#if FRUSTUM_CULLING == true
  bool passed_frustum;
#endif

  int layer = 0;

};