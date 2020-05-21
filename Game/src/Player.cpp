#include "Player.h"
#include <iostream>
Player::Player(const Vec2& _p) : GameObject()
{
  m_pos = _p;
}

void Player::update(float _dt)
{
}

void Player::draw() const
{

}
void Player::moveEvent(const Vec2& _m)
{
  m_dir = _m;
}
