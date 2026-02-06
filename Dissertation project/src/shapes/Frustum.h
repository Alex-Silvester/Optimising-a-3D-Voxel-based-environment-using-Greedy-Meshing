#pragma once

#include "../Interfaces/IDrawable.h"
#include "../camera.h"

struct Plane
{
  Plane() = default;
  Plane(const glm::vec3 &pos, const glm::vec3 &norm) : m_position(pos), m_normal(norm) {}

  glm::vec3 m_position = { 0.f,0.f,0.f };
  glm::vec3 m_normal = { 0.f,0.f,0.f, };
};

class Frustum : public IDrawable
{
public:

  ~Frustum()
  {
    m_camera_ptr = nullptr;
  }

  Frustum() = default;

  void initialise(glm::mat4 &projection, Shader *shader) override
  {
    setShader(shader, projection);
  }

  void initialise(glm::mat4 &projection) {}

  Frustum(const Camera &cam, float aspect, float fovY, float zNear, float zFar) : 
    m_camera_ptr (&cam  ),
    m_aspect     (aspect),
    m_fovY       (fovY  ),
    m_zNear      (zNear ),
    m_zFar       (zFar  )
  {

    always_rendered = true;

    updateFaces();

    setColor({ 0, 0, 1 });
  }

  void updateFaces()
  {
    //side lengths of the far plane
    const float half_vertical_side_length = m_zFar * tanf(m_fovY * .5f);
    const float half_horizontal_side_length = half_vertical_side_length * m_aspect;

    //camera forward vector scaled by the far distance
    const glm::vec3 scaled_forward_vector = m_zFar * m_camera_ptr->Front;

    const glm::vec3 &cam_pos = m_camera_ptr->Position;

    m_near_face = { cam_pos + m_zNear * m_camera_ptr->Front, m_camera_ptr->Front };
    m_far_face =  { cam_pos + scaled_forward_vector,        -m_camera_ptr->Front };

    m_right_face = { cam_pos, glm::cross(scaled_forward_vector - m_camera_ptr->Right * half_horizontal_side_length, m_camera_ptr->Up) };
    m_left_face =  { cam_pos, glm::cross(m_camera_ptr->Up, scaled_forward_vector + m_camera_ptr->Right * half_horizontal_side_length) };

    m_up_face =   { cam_pos, glm::cross(m_camera_ptr->Right, scaled_forward_vector - m_camera_ptr->Up * half_vertical_side_length) };
    m_down_face = { cam_pos, glm::cross(scaled_forward_vector + m_camera_ptr->Up * half_vertical_side_length, m_camera_ptr->Right) };

    updateVertices(half_vertical_side_length, half_horizontal_side_length, scaled_forward_vector);
  }

private:

  void updateVertices(float half_vertical_size, float half_horizontal_size, const glm::vec3 &far_vec)
  {
    const glm::vec3 near_top_left     = m_near_face.m_position +  m_camera_ptr->Right +  m_camera_ptr->Up;
    const glm::vec3 near_top_right    = m_near_face.m_position + -m_camera_ptr->Right +  m_camera_ptr->Up;
    const glm::vec3 near_bottom_right = m_near_face.m_position + -m_camera_ptr->Right + -m_camera_ptr->Up;
    const glm::vec3 near_bottom_left  = m_near_face.m_position +  m_camera_ptr->Right + -m_camera_ptr->Up;

    //const glm::vec3 far_top_left     = cam_pos + glm::vec3{-half_horizontal_size,  half_vertical_size,m_zFar}*cam_dir;
    //const glm::vec3 far_top_right    = cam_pos + glm::vec3{ half_horizontal_size,  half_vertical_size,m_zFar}*cam_dir;
    //const glm::vec3 far_bottom_right = cam_pos + glm::vec3{ half_horizontal_size, -half_vertical_size,m_zFar}*cam_dir;
    //const glm::vec3 far_bottom_left  = cam_pos + glm::vec3{-half_horizontal_size, -half_vertical_size,m_zFar}*cam_dir;

    setVertices({
                {near_top_left,    m_camera_ptr->Front},
                {near_top_right,   m_camera_ptr->Front},
                {near_bottom_right,m_camera_ptr->Front},
                {near_bottom_right,m_camera_ptr->Front},
                {near_bottom_left, m_camera_ptr->Front},
                {near_top_left,    m_camera_ptr->Front}
                }, {0,0,1});
  }

  void draw(unsigned int &VAO, unsigned int &VBO, glm::mat4 &view, DrawWindow &window) override
  {
    if (!freecam_active) return;

    glDisable(GL_CULL_FACE);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    window.draw(m_vertices, m_shader);
    glEnable(GL_CULL_FACE);

    if (window.isWireFrame())
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
  }

private:

  Plane m_near_face;
  Plane m_far_face;

  Plane m_left_face;
  Plane m_right_face;

  Plane m_up_face;
  Plane m_down_face;

  const Camera *m_camera_ptr = nullptr;
  float m_aspect = 0;
  float m_fovY = 0;
  float m_zNear = 0;
  float m_zFar = 0;

};