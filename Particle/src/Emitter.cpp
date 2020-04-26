#include "Emitter.h"
#include <iostream>
#include <ngl/Random.h>
#include <ngl/NGLStream.h>
#include <ngl/SimpleVAO.h>
#include <ngl/VAOFactory.h>


Emitter::Emitter(size_t _numParticles, ngl::Vec3 _pos)
{
	m_pos = _pos;
	std::cout << m_pos << '\n';
	m_particles.resize(_numParticles);
	for (auto& p : m_particles)
	{
		resetParticle(p);
	}
	m_vao = ngl::VAOFactory::createVAO(ngl::simpleVAO, GL_POINTS);
}

void Emitter::resetParticle(Particle& io_p)
{
	auto rng = ngl::Random::instance();
	io_p.pos = m_pos;
	io_p.dir = rng->getRandomVec3();
	io_p.dir.m_y = std::abs(io_p.dir.m_y)+0.1f;
	io_p.colour = rng->getRandomColour3();
	io_p.maxLife = static_cast<int>(rng->randomPositiveNumber(100));
	io_p.life = 0;
}
void Emitter::update()
{
	auto rng = ngl::Random::instance();

	for (auto& p : m_particles)
	{
		p.pos += p.dir;
		if (++p.life >= p.maxLife)
		{
			resetParticle(p);
		}
	}

}
void Emitter::draw() const
{
	if (m_particles.size() == 0)
	{
		return;
	}
	m_vao->bind();
	m_vao->setData(ngl::SimpleVAO::VertexData(m_particles.size()*sizeof(Particle),
		m_particles[0].pos.m_x));
	m_vao->setVertexAttributePointer(0, 3, GL_FLOAT, sizeof(Particle), 0);
	m_vao->setVertexAttributePointer(1, 3, GL_FLOAT, sizeof(Particle), 6);
	m_vao->setNumIndices(m_particles.size());
	m_vao->draw();
	m_vao->unbind();
}

void Emitter::up(float _dy)
{
	m_pos.m_y += _dy;
}
void Emitter::down(float _dy)
{
	m_pos.m_y -= _dy;
}
void Emitter::left(float _dx)
{
	m_pos.m_x -= _dx;
}
void Emitter::right(float _dx)
{
	m_pos.m_x += _dx;

}
void Emitter::in(float _dz)
{
	m_pos.m_z -= _dz;
}
void Emitter::out(float _dz)
{
	m_pos.m_z += _dz;
}

void Emitter::addParticle()
{
	Particle p;
	resetParticle(p);
	m_particles.push_back(p);
}
void Emitter::removeParticle()
{
	if(m_particles.size() !=0)
		m_particles.pop_back();
}
void Emitter::clearParticles()
{
	m_particles.clear();
}







