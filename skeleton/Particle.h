#pragma once
#include "Vector3D.h"
#include "Scene.h"
#include "RenderUtils.hpp"

enum class Integrador {
	EULER,
	EULER_SEMI
};

class Particle
{
public:
	Particle(Vector3D Pos, Vector3D Vel, Vector3D acelleration, float Damping, Integrador Tipo = Integrador::EULER);
	~Particle();

	void integrate(double t);

private:
	Vector3D vel;
	physx::PxTransform pose;
	RenderItem* renderItem;
	Vector3D ace;
	float damping; // Factor de amortiguamiento para simular fricción o resistencia del aire
	Integrador tipo;
};

