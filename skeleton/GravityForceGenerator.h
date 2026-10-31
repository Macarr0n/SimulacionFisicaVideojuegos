#pragma once
#include "ForceGenerator.h"

// class base de todos los generadores de fuerzas
class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(Vector3D gravity) : m_gravity(gravity) {}

	void updateForce(Particle* particle, double t) override {
		// F = m * g, al dividir luego entre la masa queda g para todas
		Vector3D force = m_gravity * particle->getMass();
		particle->addForce(force);
	}
private:
	Vector3D m_gravity;
};