#include "Particle.h"
#include <cmath>

Particle::Particle(Vector3D Pos, Vector3D Vel, Vector3D Acc, float Damping, Integrator Type,
	float Mass, Vector4 Color, float Radius, double LifeTime) {
	pose = physx::PxTransform(Pos);
	vel = Vel;
	acc = Acc;
	damping = Damping;
	type = Type;
	mass = Mass;
	lifeTime = LifeTime;

	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(Radius));
	renderItem = new RenderItem(shape, &pose, Color);
}

Particle::~Particle() {
	renderItem->release();   // lo quita del render y lo borra
	renderItem = nullptr;
}

void Particle::integrate(double t) {
	float dt = static_cast<float>(t);
	lifeTime -= t;

	switch (type) {
	case Integrator::EULER:
		// primero la posición, con la velocidad vieja
		pose.p += vel * dt;
		vel += acc * dt;
		break;
	case Integrator::EULER_SEMI:
		// primero la velocidad, así la posición usa la nueva
		vel += acc * dt;
		pose.p += vel * dt;
		break;
	}

	// damping^dt para que frene lo mismo vaya a los fps que vaya
	vel = vel * pow(damping, dt);
}
