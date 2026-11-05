#pragma once
#include "Particles/Particle.h"
#include <list>
#include <random>

// clase base de todos los generadores
class ParticleGenerator
{
public:
	ParticleGenerator(int perFrame, const ParticleProperties& props)
		: m_perFrame(perFrame), m_model(Vector3D(), Vector3D(), Vector3D(), props, false) {}
	virtual ~ParticleGenerator() = default;

	bool isActive() const { return m_active; }
	void setActive(bool active) { m_active = active; }

	// mete en la lista las partículas que se crean en este frame
	void generateParticles(std::list<Particle*>& particles, const Vector3D& gravity) {
		for (int i = 0; i < m_perFrame; i++) {
			Particle* p = new Particle(m_model);
			p->setPos(randomPos());
			p->setVel(randomVel());
			p->setAcc(gravity);
			particles.push_back(p);
		}
	}

protected:
	// cada generador decide cómo saca la posición y la velocidad aleatoriasW
	virtual Vector3D randomPos() = 0;
	virtual Vector3D randomVel() = 0;

	std::default_random_engine m_randomEngine{ std::random_device{}() };   // da los números aleatorios a las distribuciones

private:
	Particle m_model;				// partícula modelo
	bool m_active = true;			// si está apagado no crea partículas
	int m_perFrame = 0;				// articulas por frame
	ParticleProperties m_props;		// masa, color, vida... de las particulas que crea
};
