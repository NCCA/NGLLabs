#include "Arena.h"
#include <iostream>
#include <ngl/Image.h>


Arena::Arena(unsigned int _w, unsigned int _h)
{
  m_width = _w;
  m_height = _h;  
  m_items.resize(m_width * m_height);
}


Arena::Arena(const std::string& _filename)
{
  ngl::Image img(_filename);
  m_width = img.width();
  m_height = img.height();
  m_items.resize(m_width * m_height);

  for (unsigned int y = 0; y < m_height; ++y)
  {
    for (unsigned int x = 0; x < m_width; ++x)
    {
      if (img.getColour(x, y) == ngl::Vec4::zero())
      {
        setItem(x, y, ItemType::Wall);
      }
    }
  }
}

void Arena::draw() const
{
  
#ifdef WIN32
  system("cls");
#else
  system("clear");
#endif

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
        case ItemType::Wall: std::cout << 'W'; break;
      }
    }
    std::cout << '\n';
  }

}
void Arena::update(float _dt)
{
  std::cout << "update \n";
}
void Arena::setItem(unsigned int _x, unsigned int _y, ItemType _type)
{
  size_t index = _y * m_width + _x;
  m_items[index].type = _type;
}
