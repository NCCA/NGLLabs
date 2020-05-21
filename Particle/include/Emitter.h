#ifndef EMITTER_H_
#define EMITTER_H_
#include <vector>
#include "Particle.h"
#include <ngl/AbstractVAO.h>
#include <memory>
class Emitter
{
public :
	Emitter(size_t _numParticles, ngl::Vec3 _pos=0, float _spread=1.5f,ngl::Vec3 _emitDir=ngl::Vec3(0.0f,10.0f,0.0f));
	~Emitter() = default;
	void update(float _deltaT);
	void draw() const;
	void up(float _dy);
	void down(float _dy);
	void left(float _dx);
	void right(float _dy);
	void in(float _dz);
	void out(float _dz);
	void addParticle();
	void removeParticle();
	void clearParticles();
	void changeSpread(float _spread);
	void resetParticles();
	void toggleSingleShot();

private :
	void resetParticle(Particle& io_p);
	ngl::Vec3 randomVectorOnSphere(float _radius = 1.0f);
	std::vector<Particle> m_particles;
	std::unique_ptr<ngl::AbstractVAO> m_vao;
	ngl::Vec3 m_pos;
	float m_spread = 1.5f;
	ngl::Vec3 m_emitDir=ngl::Vec3(0.0f,10.0f,0.0f);
	size_t m_numParticles = 1000;
	bool m_singleShot = false;

};

#endif