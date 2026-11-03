#pragma once
#include "ForceGenerators/ForceGenerator.h"

// viento uniforme dentro de la esfera
class WindForceGenerator : public ForceGenerator
{
public:
	WindForceGenerator(Vector3D windVel, Vector3D center, float radius, float k1, float k2 = 0.f)
		: m_windVel(windVel), m_center(center), m_radius(radius), m_k1(k1), m_k2(k2) {}

	void updateForce(Particle* p, double dt) override {
		if (!isInside(p)) return;

		// velocidad del aire respecto a la partícula
		Vector3D diff = windVelAt(p) - p->getVel();
		p->addForce(diff * m_k1 + diff * (m_k2 * diff.magnitude()));
	}

protected:
	Vector3D m_center;		// centro de la zona de viento
	float m_radius;				// radio de la zona de viento
	float m_k1, m_k2;
	Vector3D m_windVel;

	// velocidad del viento donde está la partícula (el torbellino cambiará esto)
	virtual Vector3D windVelAt(Particle* p) {
		return m_windVel;
	}
	bool isInside(const Particle* p) const {
		return (p->getPos() - m_center).magnitude() < m_radius;
	}
};