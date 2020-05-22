#include "RenderGlobals.h"
#include <ngl/Util.h>

ngl::Mat4 RenderGlobals::s_view = ngl::Mat4();
ngl::Mat4 RenderGlobals::s_project = ngl::Mat4();


void RenderGlobals::setView(const ngl::Vec3& _eye, const ngl::Vec3& _look, const ngl::Vec3& _up)
{
  s_view = ngl::lookAt(_eye, _look, _up);
}

void RenderGlobals::setProjection(float _fov, float _aspect, float _near, float _far)
{
  s_project = ngl::perspective(_fov, _aspect, _near, _far);
}


ngl::Mat4 RenderGlobals::getViewMatrix()
{
  return s_view;
}
ngl::Mat4 RenderGlobals::getProjectMatrix()
{
  return s_project;
}
ngl::Mat4 RenderGlobals::getVPMatrix()
{
  return s_project * s_view;
}


