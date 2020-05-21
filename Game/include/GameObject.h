#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_
#include "Vec2.h"

class GameObject
{
public :
  GameObject() = default;
  ~GameObject() = default;
  virtual void update(float _dt);
  virtual void draw() const;
  virtual void moveEvent(const Vec2& _m);
private :

};


#endif