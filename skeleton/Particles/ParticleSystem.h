#pragma once
#include "ParticleGenerators/ParticleGenerator.h"
#include "ForceGenerators/ForceGenerator.h"

#include <list>
#include <vector>
#include <algorithm>

// guarda los generadores y las partículas: las crea, las mueve y las borra
class ParticleSystem
{
public:
	ParticleSystem(Vector3D gravity, Vector3D center, float radius)
		: m_gravity(gravity), m_center(center), m_radius(radius) {}

	// borra sus partículas y sus generadores de partículas
	// los generadores de fuerzas no son suyos
	~ParticleSystem() {
		for (Particle* p : m_particles) delete p;
		for (ParticleGenerator* g : m_generators) delete g;
	}

	void addGenerator(ParticleGenerator* generator) {
		m_generators.push_back(generator);
	}

	void addForceGenerator(ForceGenerator* forceGenerator) {
		m_forceGenerators.push_back(forceGenerator);
	}

	// deja de afectarle, pero no la borra, puede seguir en otros sistemas
	void removeForceGenerator(ForceGenerator* forceGenerator) {
		auto it = std::find(m_forceGenerators.begin(), m_forceGenerators.end(), forceGenerator);
		if (it != m_forceGenerators.end()) {
			m_forceGenerators.erase(it);
		}
	}

	// enciende y apaga el generador número index
	// devuelve true si se ha quedado encendido
	bool toggleGenerator(int index) {
		ParticleGenerator* g = m_generators[index];
		g->setActive(!g->isActive());
		return g->isActive();
	}

	// para meter partículas sueltas sin generador
	void addParticle(Particle* p) {
		m_particles.push_back(p);
	}

	void update(double dt) {
		for (ParticleGenerator* g : m_generators) {
			if (g->isActive()) {
				g->generateParticles(m_particles, m_gravity);
			}
		}

		// primero todas calculan sus fuerzas con las posiciones de este frame
		for (Particle* p : m_particles) {
			p->applyForces(m_forceGenerators, dt);
		}

		// luego se mueven todas
		auto it = m_particles.begin();
		while (it != m_particles.end()) {
			Particle* p = *it;
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
	Vector3D m_gravity;            // la misma para todas las partículas
	Vector3D m_center;             // centro de la zona de acción
	float m_radius;                     // radio de la zona de acción
	std::vector<ForceGenerator*> m_forceGenerators;

	bool isOutOfBounds(const Particle* p) const {
		return (p->getPos() - m_center).magnitude() > m_radius;
	}
};
