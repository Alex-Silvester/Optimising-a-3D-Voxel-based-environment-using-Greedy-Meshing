#pragma once

#include "WindowBase.h"
#include "../camera.h"
#include "../Interfaces/IDrawable.h"

#include "../Helpers/Settings.h"

class DrawWindow : public WindowBase
{
public:

  DrawWindow() = default;

  void draw(IDrawable& drawable)
  {
    glm::mat4 view = camera.GetViewMatrix();
    drawable.draw(VAO, VBO, view, *this);
  }

  void draw(const std::vector<float>& vertices, Shader* shader)
  {
    glm::mat4 view = camera.GetViewMatrix();

    shader->use();
    shader->setMat4("view", view);
    shader->setVec3("position", {0,0,0});

    // render
    glBindVertexArray(VAO);

    //explicitly bind the VBO
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // calculate the model matrix for each object and pass it to shader before drawing
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    model = glm::translate(model, glm::vec3(0.f));
    shader->setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 9);
  }

  bool initialise(float size_x = SCREEN_WIDTH, float size_y = SCREEN_HEIGHT, const char *name = "") override
  {
    if (!WindowBase::initialise(size_x, size_y, name))
    {
      return false;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

#if CULL_FACES == true
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
#endif

    glEnable(GL_MULTISAMPLE);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //position(3 floats), colour(3 floats), normal(3 floats)

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return true;
  }

  const Camera& getCamera() const
  {
    return camera;
  }

  bool paused()
  {
    return m_paused;
  }

private:

	void mouseEvent(double xposIn, double yposIn)override
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (m_first_mouse)
		{
			m_lastX = xpos;
			m_lastY = ypos;
			m_first_mouse = false;
		}

		float xoffset = xpos - m_lastX;
		float yoffset = m_lastY - ypos; // reversed since y-coordinates go from bottom to top

		m_lastX = xpos;
		m_lastY = ypos;

    if(!m_paused)
    {
      camera.ProcessMouseMovement(xoffset, yoffset);
    }
	}

	void processInput() override
	{
    GLint polygonMode;
    glGetIntegerv(GL_POLYGON_MODE, &polygonMode);

    //close window
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(m_window, true);
    }

    //visuals type
    if (glfwGetKey(m_window, GLFW_KEY_F5) == GLFW_PRESS && !f5_pressed)
    {
      f5_pressed = true;
      if (polygonMode == GL_FILL)
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }
      else if (polygonMode == GL_LINE)
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
    }
    if (glfwGetKey(m_window, GLFW_KEY_F5) == GLFW_RELEASE && f5_pressed)
    {
      f5_pressed = false;
    }

    //Screen pause
    if (keyPressed(GLFW_KEY_ENTER) && !enter_pressed)
    {
      enter_pressed = true;
      m_paused = !m_paused;
      
      glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      if (m_paused)
      {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      }
    }
    if (keyReleased(GLFW_KEY_ENTER))
    {
      enter_pressed = false;
    }

    if (m_paused) return;

    //forward/backward movement
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    {
      camera.ProcessKeyboard(FORWARD, m_delta_time);
    }
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    {
      camera.ProcessKeyboard(BACKWARD, m_delta_time);
    }

    //left/right movement
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
      camera.ProcessKeyboard(LEFT, m_delta_time);
    }
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {
      camera.ProcessKeyboard(RIGHT, m_delta_time);
    }

    //up/down movement
    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
      camera.ProcessKeyboard(UP, m_delta_time);
    }
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
      camera.ProcessKeyboard(DOWN, m_delta_time);
    }

    //sprint
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
      camera.sprint_active = true;
    }
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
    {
      camera.sprint_active = false;
    }

    //cursor type
    if (glfwGetKey(m_window, GLFW_KEY_TAB) == GLFW_PRESS && !tab_pressed)
    {
      tab_pressed = true;
      if (glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
      {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
      else
      {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      }
    }
    if (glfwGetKey(m_window, GLFW_KEY_TAB) == GLFW_RELEASE)
    {
      tab_pressed = false;
    }

  }

  bool keyPressed(int key)
  {
    return glfwGetKey(m_window, key) == GLFW_PRESS;
  }

  bool keyReleased(int key)
  {
    return glfwGetKey(m_window, key) == GLFW_RELEASE;
  }

private:

	float m_lastX = 0;
	float m_lastY = 0;
	bool m_first_mouse = true;

  bool  space_pressed = false;
  bool tab_pressed = false;
  bool f5_pressed = false;
  bool enter_pressed = false;

	Camera camera{ glm::vec3(10.f, 7.5f, -10.f) , {0.f,1.f,0.f}, 90.f, 0.f };

  unsigned int VAO = 0, VBO = 0;

  bool m_paused = false;
};