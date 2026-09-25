#include "Particle.h"
#include <cmath>

Particle::Particle(Vector3D Pos, Vector3D Vel, Vector3D acelleration) {
	pose = physx::PxTransform(Pos);
	vel = Vel;
	ace = acelleration;

	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(1.0f));
	renderItem = new RenderItem(shape, &pose, Vector4(1.f, 0.f, 0.f, 1.f));
}

Particle::~Particle() {
	renderItem->release(); // Deregistra y destruye el item
	renderItem = nullptr;
}

void Particle::integrate(double t) {
	float dt = static_cast<float>(t);
	/*
	vel.x += ace.x * dt;
	vel.y = ace.y * dt;
	vel.z = ace.z * dt;
	*/
}
