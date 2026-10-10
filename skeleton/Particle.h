#pragma once
#include "Vector3D.h"
#include "Scene.h"
#include "RenderUtils.hpp"

// cómo se mueve la partícula en cada frame
enum class Integrator {
	EULER,
	EULER_SEMI
};

class Particle
{
public:
	Particle(Vector3D Pos, Vector3D Vel, Vector3D Acc, float Damping, Integrator Type = Integrator::EULER,
		float Mass = 1.0f, Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), float Radius = 1.0f, double LifeTime = 5.0);
	~Particle();

	void integrate(double t);
	physx::PxVec3 getPos() const { return pose.p; }
	bool isAlive() const { return lifeTime > 0.0; }

private:
	Vector3D vel;
	physx::PxTransform pose;   // el render item lee la posición de aquí
	RenderItem* renderItem;
	Vector3D acc;
	float damping;             // entre 0 y 1, la va frenando como el rozamiento del aire
	Integrator type;
	float mass;
	double lifeTime;           // segundos que le quedan antes de morir
};
