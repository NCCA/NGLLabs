#include "Sphere.h"
#include <ngl/VAOPrimitives.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>

Sphere::Sphere(const ngl::Vec3 &_pos, float _radius) : m_pos{_pos},m_radius{_radius}
{

}

void Sphere::draw(const ngl::Mat4 &_vp) const
{
  ngl::Transformation tx;
  tx.setPosition(m_pos);
  tx.setScale(m_radius,m_radius,m_radius);
  auto shader = ngl::ShaderLib::instance();
  auto MVP = _vp * tx.getMatrix();
  shader->setUniform("MVP",MVP);
  shader->setUniform("Colour",m_colour);
  ngl::VAOPrimitives::instance()->draw("sphere");
}
void Sphere::moveRelative(float _dx,float _dy, float _dz)
{
  m_pos.m_x += _dx;
  m_pos.m_y += _dy;
  m_pos.m_z += _dz;

}
void Sphere::moveRelative(const ngl::Vec3 &_d)
{
  m_pos+=_d;
}

ngl::Vec3 Sphere::getPos() const
{
  return m_pos;
}

void Sphere::setPos(const ngl::Vec3 &_p)
{
  m_pos=_p;
}

void Sphere::setColour(const ngl::Vec4 &_c)
{
  m_colour=_c;
}
void Sphere::setColour(ngl::Real _r, ngl::Real _g, ngl::Real _b)
{
  m_colour.m_r=_r;
  m_colour.m_g=_g;
  m_colour.m_b=_b;
}

void Sphere::setRadius(ngl::Real _r)
{
  m_radius=_r;
}

bool Sphere::isColliding(const Sphere &_s)
{
  // the relative position of the spheres
  ngl::Vec3 relPos;
  //min an max distances of the spheres
  GLfloat dist;
  GLfloat minDist;
  GLfloat len;
  relPos =_s.m_pos-m_pos;
  // and the distance
  len=relPos.length();
  dist=len*len;
  minDist =_s.m_radius+m_radius;
  // if it is a hit
  if(dist <=(minDist * minDist))
  {
    return true;
  }
  else
  {
    return false;
  }
}


