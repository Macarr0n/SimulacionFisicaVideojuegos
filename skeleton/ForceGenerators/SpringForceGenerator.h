#pragma once
#include "ForceGenerators/ForceGenerator.h"

// hebra entre dos bolas: un muelle que solo empuja o tira de sus dos extremos
class SpringForceGenerator : public ForceGenerator
{
public:
	SpringForceGenerator(Particle* a, Particle* b, float k, float restLength, float damping)
		: m_a(a), m_b(b), m_k(k), m_restLength(restLength), m_damping(damping) {}

	void updateForce(Particle* p, double dt) override {
		// el sistema se lo pide a todas sus partículas, pero solo afecta a sus dos extremos
		Particle* other = nullptr;
		if (p == m_a) other = m_b;
		else if (p == m_b) other = m_a;
		else return;

		Vector3D d = other->getPos() - p->getPos();
		float length = d.magnitude();
		if (length < 0.0001f) return;   // las dos en el mismo punto: no hay dirección
		Vector3D dir = d * (1.f / length);

		// Hooke: F = k * (L - L0), hacia la otra bola si está estirado y al revés si está comprimido
		float stretch = length - m_restLength;

		// amortiguación: frena la velocidad con la que se separan o se juntan, para que no vibre siempre
		float relVel = (other->getVel() - p->getVel()).dot(dir);

		p->addForce(dir * (m_k * stretch + m_damping * relVel));
	}

	Particle* getA() const { return m_a; }
	Particle* getB() const { return m_b; }

	float getRestLength() const { return m_restLength; }

private:
	Particle* m_a;
	Particle* m_b;
	float m_k;            // dureza
	float m_restLength;   // longitud en reposo
	float m_damping;      // amortiguación
};