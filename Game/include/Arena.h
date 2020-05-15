#ifndef ARENA_H_
#define ARENA_H_
#include "GameItem.h"
#include <vector>
#include <string>
class Arena
{
public :
  Arena() = default;
  ~Arena() = default;
  Arena(unsigned int _w, unsigned int _h);
  Arena(const std::string& _filename);
  void draw() const;
  void update(float _dt);
  void setItem(unsigned int _x, unsigned int _y, ItemType _type);
private :
  unsigned int m_width=0;
  unsigned int m_height=0;
  std::vector<GameItem> m_items;

};


#endif // ARENA_H_
