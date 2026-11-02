#pragma once
#include "ForceGenerators/WindForceGenerator.h"

// torbellino: el viento gira alrededor del eje Y del centro y además sube
class WhirlwindForceGenerator : public WindForceGenerator
{
public:
	WhirlwindForceGenerator(physx::PxVec3 center, float radius, float K, float k1, float k2 = 0.f)
		: WindForceGenerator(Vector3D(), center, radius, k1, k2), m_K(K) {}

protected:
	float m_K;		// fuerza del torbellino

	// aquí el viento depende de dónde esté la partícula 
	Vector3D windVelAt(Particle* p) override {
		physx::PxVec3 d = p->getPos() - m_center;
		return Vector3D(-d.z, 50.f - d.y, d.x) * m_K;
	}
};