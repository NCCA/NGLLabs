#ifndef RENDERGLOBALS_H_
#define RENDERGLOBALS_H_

#include <ngl/Vec3.h>
#include <ngl/Mat4.h>

class RenderGlobals
{
public :
  RenderGlobals() = delete;
  ~RenderGlobals() = delete;
  static void setView(const ngl::Vec3& _eye, const ngl::Vec3& _look, const ngl::Vec3& _up);
  static void setProjection(float _fov, float _aspect, float _near = 0.1f, float _far = 100.0f);

  static ngl::Mat4 getViewMatrix();
  static ngl::Mat4 getProjectMatrix();
  static ngl::Mat4 getVPMatrix();

private :
  static ngl::Mat4 s_view;
  static ngl::Mat4 s_project;

};



#endif