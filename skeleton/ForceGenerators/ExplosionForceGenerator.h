#pragma once
#include "ForceGenerators/ForceGenerator.h"
#include <cmath>

// explosión: empuja hacia fuera desde el centro y se va apagando con el tiempo
class ExplosionForceGenerator : public ForceGenerator {
public:
	ExplosionForceGenerator(Vector3D center, float radius, float K, float tau)
		: m_center(center), m_radius(radius), m_K(K), m_tau(tau) {}

	// empieza la explosión desde cero
	void explode() {
		m_active = true;
		m_time = 0.0;
	}

	void update(double dt) override {
		if (!m_active) return;
		m_time += dt;
		// a partir de 4 tau ya casi no hace nada, así que se apaga
		if (m_time > 4.0 * m_tau) m_active = false;
	}

	void updateForce(Particle* p, double dt) override {
		if (!m_active) return;

		Vector3D d = p->getPos() - m_center;
		float r2 = d.magnitudeSquared();
		// fuera del radio o justo en el dentro
		if (r2 >= m_radius * m_radius || r2 < 0.01f) return;

		// F = K / r^2 * (x - xc) * e^(-t / tau)
		float strength = m_K / r2 * std::exp(-static_cast<float>(m_time) / m_tau);
		p->addForce(d * strength);
	}

private:
	Vector3D m_center;
	float m_radius;			// R
	float m_K;				// intensidad
	float m_tau;			// constante de tiempo
	double m_time = 0.0;	// tiempo desde que explotó
	bool m_active = false;	
};