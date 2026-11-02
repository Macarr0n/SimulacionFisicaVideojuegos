#pragma once
#include "Particles/Particle.h"

// clase base de todos los generadores de fuerzas
class ForceGenerator {
public:
	virtual ~ForceGenerator() = default;

	// calcula la fuerza que le toca a la partícula y se la suma
	virtual void updateForce(Particle* particle, double t) = 0;

	// una vez por frame, para los que cambian con el tiempo (la explosión)
	virtual void update(double dt) {}
};