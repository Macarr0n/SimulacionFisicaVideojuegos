#pragma once
#include "Particle.h"

// clase base de todos los generadores de fuerzas
class ForceGenerator {
public:
	virtual ~ForceGenerator() = default;

	// calcula la fuerza que le toca a la partícula y se la suma
	virtual void updateForce(Particle* particle, double t) = 0;
};