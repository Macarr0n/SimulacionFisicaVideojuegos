#pragma once
#include "ForceGenerators/ForceGenerator.h"

// muelle entre una bola y un punto fijo (para pegar la base de la pirámide al suelo)
class AnchoredSpringForceGenerator : public ForceGenerator
{
public:
	AnchoredSpringForceGenerator(Particle* p, Vector3D anchor, float k, float damping)
		: m_particle(p), m_anchor(anchor), m_k(k), m_damping(damping) {}

	void updateForce(Particle* p, double dt) override {
		if (p != m_particle) return;   // solo afecta a su bola

		// longitud natural 0: tira hacia el punto fijo, y frena para que no se quede rebotando
		p->addForce((m_anchor - p->getPos()) * m_k - p->getVel() * m_damping);
	}

private:
	Particle* m_particle;
	Vector3D m_anchor;   // punto del suelo al que está pegada
	float m_k;
	float m_damping;
};