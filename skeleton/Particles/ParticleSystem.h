#pragma once
#include "ParticleGenerators/ParticleGenerator.h"
#include "ForceGenerators/ForceGenerator.h"

#include <list>
#include <vector>

// guarda los generadores y las partículas: las crea, las mueve y las borra
class ParticleSystem
{
public:
	ParticleSystem(physx::PxVec3 gravity, physx::PxVec3 center, float radius)
		: m_gravity(gravity), m_center(center), m_radius(radius) {}

	// el sistema es el dueño de todo, así que lo borra él
	~ParticleSystem() {
		for (Particle* p : m_particles) delete p;
		for (ParticleGenerator* g : m_generators) delete g;
		for (ForceGenerator* f : m_forceGenerators) delete f;
	}

	void addGenerator(ParticleGenerator* generator) {
		m_generators.push_back(generator);
	}

	void addForceGenerator(ForceGenerator* forceGenerator) {
		m_forceGenerators.push_back(forceGenerator);
	}

	// para meter partículas sueltas sin generador
	void addParticle(Particle* p) {
		m_particles.push_back(p);
	}

	void update(double dt) {
		for (ParticleGenerator* g : m_generators) {
			g->generateParticles(m_particles, m_gravity);
		}

		auto it = m_particles.begin();
		while (it != m_particles.end()) {
			Particle* p = *it;
			for (ForceGenerator* f : m_forceGenerators) {
				f->updateForce(p, dt);
			}
			p->integrate(dt);
			if (!p->isAlive() || isOutOfBounds(p)) {
				delete p;
				it = m_particles.erase(it);   // erase ya devuelve la siguiente, por eso aquí no hay ++it
			}
			else {
				++it;
			}
		}
	}

private:
	std::vector<ParticleGenerator*> m_generators;
	std::list<Particle*> m_particles;   // lista porque se borran del medio todo el rato
	physx::PxVec3 m_gravity;            // la misma para todas las partículas
	physx::PxVec3 m_center;             // centro de la zona de acción
	float m_radius;                     // radio de la zona de acción
	std::vector<ForceGenerator*> m_forceGenerators;

	bool isOutOfBounds(const Particle* p) const {
		return (p->getPos() - m_center).magnitude() > m_radius;
	}
};
