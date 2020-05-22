#include "Player.h"
#include <iostream>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include "RenderGlobals.h"
Player::Player(const Vec2& _p, unsigned int _w, unsigned int _h) : GameObject()
{
  m_pos = _p;
  m_width = _w;
  m_height = _h;
}

void Player::update(float _dt)
{
}

void Player::draw() const
{
  float halfZ = -(m_height / 2.0f);
  float halfX = -(m_width / 2.0f);
  ngl::Transformation tx;
  tx.setPosition(halfX + m_pos.x, 0.0f, halfZ + m_pos.y);
  tx.setRotation(0, 90, 0);
  auto shader = ngl::ShaderLib::instance();
  shader->setUniform("MVP", RenderGlobals::getVPMatrix() * tx.getMatrix());
  shader->setUniform("Colour", 0.0f, 1.0f, 0.0f, 1.0f);

  ngl::VAOPrimitives::instance()->draw(ngl::troll);
}
void Player::moveEvent(const Vec2& _m)
{
  m_dir = _m;
}
