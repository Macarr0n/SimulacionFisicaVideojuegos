#pragma once
#include "Particle.h"
#include <list>
#include <random>

// clase base de todos los generadores
class ParticleGenerator
{
public:
	virtual ~ParticleGenerator() = default;

	// mete en la lista las partículas que se crean en este frame
	virtual void generateParticles(std::list<Particle*>& particles, const physx::PxVec3& gravity) = 0;

protected:
	std::default_random_engine m_randomEngine;   // da los números aleatorios a las distribuciones
};
