#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <ngl/Vec3.h>

struct Particle
{
	ngl::Vec3 pos;
	ngl::Vec3 dir;
	ngl::Vec3 colour;
	unsigned int life = 0;
	unsigned int maxLife = 0;
};	



#endif