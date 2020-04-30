#include "Emitter.h"
#include <iostream>
#include <ngl/Random.h>
#include <ngl/NGLStream.h>
#include <ngl/SimpleVAO.h>
#include <ngl/VAOFactory.h>


Emitter::Emitter(size_t _numParticles, ngl::Vec3 _pos, float _spread,ngl::Vec3 _emitDir)
{
	m_pos = _pos;
	m_spread = _spread;
	m_emitDir = _emitDir;
	m_numParticles = _numParticles;
	std::cout << m_pos << '\n';
	m_particles.resize(_numParticles);
	for (auto& p : m_particles)
	{
		resetParticle(p);
	}
	m_vao = ngl::VAOFactory::createVAO(ngl::simpleVAO, GL_POINTS);
}

void Emitter::resetParticles()
{
	m_particles.resize(m_numParticles);
	for (auto& p : m_particles)
	{
		resetParticle(p);
	}
}

void Emitter::toggleSingleShot()
{
	m_singleShot ^= true;
}

void Emitter::resetParticle(Particle& io_p)
{

	auto rng = ngl::Random::instance();
	io_p.pos = m_pos;
	io_p.dir = m_emitDir * rng->randomPositiveNumber(1.0f) + randomVectorOnSphere() * m_spread;
	io_p.dir.m_y = std::abs(io_p.dir.m_y);
	io_p.colour = rng->getRandomColour3();
	io_p.maxLife = static_cast<int>(rng->randomPositiveNumber(5000))+500;
	io_p.life = 0;
}
void Emitter::update(float _deltaT)
{
	ngl::Vec3 gravity(0.0f, -9.81f, 0.0f);
	for (auto& p : m_particles)
	{
		p.dir += gravity * _deltaT * 0.5;
		p.pos += p.dir * _deltaT;
		if (!m_singleShot)
		{
			if (++p.life >= p.maxLife || p.pos.m_y <= 0.0f)
			{
				resetParticle(p);
			}
		}
	
	
	}
	if (m_singleShot)
	{
		m_particles.erase(
			std::remove_if(std::begin(m_particles), std::end(m_particles),
				[](const Particle& p) { return (p.pos.m_y <= 0 || p.life >= p.maxLife); }),
			std::end(m_particles));
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

void Emitter::changeSpread(float _spread)
{
	m_spread = _spread;
}

ngl::Vec3 Emitter::randomVectorOnSphere(float _radius )
{
	auto rng = ngl::Random::instance();
	float phi = rng->randomPositiveNumber(static_cast<float>(M_PI * 2.0f));
	float costheta = rng->randomNumber();
	float u = rng->randomPositiveNumber();
	float theta = acos(costheta);
	float r = _radius * std::cbrt(u);
	return ngl::Vec3(r * sin(theta) * cos(phi),
									 r * sin(theta) * sin(phi),
									 r * cos(theta)
									);
}






