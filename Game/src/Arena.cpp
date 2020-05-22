#include "Arena.h"
#include <iostream>
#include <ngl/Image.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include "RenderGlobals.h"

#include "Player.h"

const static char* Player1 = "Player1";
const static char* Wall = "Wall";
const static char* Empty = "Empty";


Arena::Arena(unsigned int _w, unsigned int _h)
{
  m_width = _w;
  m_height = _h;  
  m_items.resize(m_width * m_height);
  createDefaultObjects();
}


Arena::Arena(const std::string& _filename)
{
  ngl::Image img(_filename);
  m_width = img.width();
  m_height = img.height();
  m_items.resize(m_width * m_height);
  createDefaultObjects();
  for (unsigned int y = 0; y < m_height; ++y)
  {
    for (unsigned int x = 0; x < m_width; ++x)
    {
      if (img.getColour(x, y) == ngl::Vec4::zero())
      {
        setItem(x, y, ItemType::Wall,m_objects[Wall]);
      }
      else
      {
        setItem(x, y, ItemType::Empty, m_objects[Empty]);
      }
    }
  }
  
    setItem(4, 4, ItemType::Player, m_objects[Player1]);
    ngl::VAOPrimitives::instance()->createTrianglePlane("floor", m_width, m_height, 1.0f, 1.0f, ngl::Vec3::up());
}


void Arena::createDefaultObjects()
{
  m_objects[Wall] = new GameObject();
  m_objects[Empty] = new GameObject();
  m_objects[Player1] = new Player(Vec2(4,4),m_width,m_height);

}

void Arena::keyEvent( int _x,  int _y)
{
  std::cout << _x << ' ' << _y << '\n';
  m_objects[Player1]->moveEvent(Vec2(_x, _y));
}


void Arena::draw() const
{
  auto shader = ngl::ShaderLib::instance();
  shader->use(ngl::nglDiffuseShader);
#ifdef WIN32
  system("cls");
#else
  system("clear");
#endif
  shader->setUniform("Colour", 1.0f, 0.0f, 0.0f, 1.0f);
  size_t index = 0;
  for (size_t y = 0; y < m_height; ++y)
  {
    for (size_t x = 0; x < m_width; ++x)
    {
      switch (m_items[index++].type)
      {
        case ItemType::Empty: std::cout << ' '; break;
        case ItemType::Player: std::cout << 'P'; break;
        case ItemType::Fruit: std::cout << 'F'; break;
        case ItemType::Wall: std::cout << 'W'; drawWall(x, y);  break;
      }
    }
    std::cout << '\n';
  }

  for (auto& o : m_objects)
  {
    o.second->draw();
  }
  shader->use(ngl::nglCheckerShader);
  ngl::Transformation tx;
  tx.setPosition(0.0f, -0.6f, 0.0f);
  shader->setUniform("MVP", RenderGlobals::getVPMatrix() * tx.getMatrix());
  ngl::VAOPrimitives::instance()->draw("floor");
}
void Arena::update(float _dt)
{
  std::cout << "update \n";

  for (auto& o : m_objects)
  {
    o.second->update(_dt);
  }
  Player* p = static_cast<Player *>(m_objects[Player1]);
  auto pos = p->getPos();
  auto dir = p->getDir();
  Vec2 newPos=pos;

  auto getItem = [=](unsigned int _x, unsigned int _y)
  {
    size_t index = _y * m_width + _x;
    return m_items[index].type;
  };

  if (dir.x != 0 && getItem(pos.x + dir.x, pos.y + dir.y) == ItemType::Empty)
  {
    newPos.x = pos.x + dir.x;
  }

  if (dir.y != 0 && getItem(pos.x + dir.x, pos.y + dir.y) == ItemType::Empty)
  {
    newPos.y = pos.y + dir.y;
  }
  p->setPos(newPos);
  setItem(pos.x, pos.y, ItemType::Empty, m_objects[Empty]);
  setItem(newPos.x, newPos.y, ItemType::Player, m_objects[Player1]);


}
void Arena::setItem(unsigned int _x, unsigned int _y, ItemType _type, GameObject * _obj)
{
  size_t index = _y * m_width + _x;
  m_items[index].type = _type;
  m_items[index].obj = _obj;
}


void Arena::drawWall(unsigned int _x, unsigned int _y) const
{
  float halfZ = -(m_height / 2.0f);
  float halfX = -(m_width / 2.0f);
  ngl::Transformation tx;
  tx.setPosition(halfX + _x, 0.0f, halfZ + _y);
  auto shader = ngl::ShaderLib::instance();
  shader->setUniform("MVP", RenderGlobals::getVPMatrix() * tx.getMatrix());
  ngl::VAOPrimitives::instance()->draw(ngl::cube);
}
