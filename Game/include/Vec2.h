#ifndef VEC2_H_
#define VEC2_H_



struct Vec2
{
  Vec2() = default;
  Vec2(unsigned int _x, unsigned int _y) : x{_x},y{_y}{}
  unsigned int x=0;
  unsigned int y=0;
};

#endif
