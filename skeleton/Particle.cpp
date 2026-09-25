#include "Particle.h"
#include <cmath>

Particle::Particle(Vector3D Pos, Vector3D Vel, Vector3D acelleration, float Damping, Integrador Tipo) {
	pose = physx::PxTransform(Pos);
	vel = Vel;
	ace = acelleration;
	damping = Damping;
	tipo = Tipo;

	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(1.0f));
	renderItem = new RenderItem(shape, &pose, Vector4(1.f, 0.f, 0.f, 1.f));
}

Particle::~Particle() {
	renderItem->release(); // Deregistra y destruye el item
	renderItem = nullptr;
}

void Particle::integrate(double t) {
	float dt = static_cast<float>(t);

	switch (tipo) {
	case Integrador::EULER:
		pose.p += vel * dt;     
		vel += ace * dt;     
		break;
	case Integrador::EULER_SEMI:
		vel += ace * dt;        
		pose.p += vel * dt;     
		break;
	}

	vel = vel * pow(damping, dt);   // el damping se aplica igual en los dos
}
