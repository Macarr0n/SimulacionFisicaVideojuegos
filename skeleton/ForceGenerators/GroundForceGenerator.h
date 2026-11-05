#pragma once
#include "ForceGenerators/ForceGenerator.h"

// suelo blando: si una bola se hunde, la empuja hacia arriba como un muelle y la frena al deslizar
class GroundForceGenerator : public ForceGenerator
{
public:
	GroundForceGenerator(float height, float ballRadius, float k, float damping, float friction)
		: m_height(height), m_ballRadius(ballRadius), m_k(k), m_damping(damping), m_friction(friction) {}

	void updateForce(Particle* p, double dt) override {
		// cuánto se ha metido la bola dentro del suelo
		float depth = m_height + m_ballRadius - p->getPos().y;
		if (depth <= 0.f) return;

		Vector3D v = p->getVel();
		float up = m_k * depth - m_damping * v.y;
		if (up < 0.f) up = 0.f;   // el suelo solo empuja, nunca tira hacia abajo

		// rozamiento: frena lo que se mueve en horizontal
		p->addForce(Vector3D(-v.x * m_friction, up, -v.z * m_friction));
	}

private:
	float m_height;       // y del suelo
	float m_ballRadius;
	float m_k;            // dureza del suelo
	float m_damping;      // para que no rebote como una pelota
	float m_friction;
};