#pragma once
#include "Vector3D.h"
#include "Scene.h"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3D Pos, Vector3D Vel, Vector3D acelleration);
	~Particle();

	void integrate(double t);

private:
	Vector3D vel;
	physx::PxTransform pose;
	RenderItem* renderItem;
	Vector3D ace;

};

