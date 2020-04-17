#ifndef EMITTER_H_
#define EMITTER_H_
#include <vector>
#include "Particle.h"
#include <ngl/AbstractVAO.h>
#include <memory>
class Emitter
{
public :
	Emitter(size_t _numParticles, ngl::Vec3 _pos=0);
	~Emitter() = default;
	void update();
	void draw() const;
	void up(float _dy);
	void down(float _dy);
	void left(float _dx);
	void right(float _dy);
	void in(float _dz);
	void out(float _dz);

private :
	void resetParticle(Particle& io_p);
	std::vector<Particle> m_particles;
	std::unique_ptr<ngl::AbstractVAO> m_vao;
	ngl::Vec3 m_pos;
};

#endif