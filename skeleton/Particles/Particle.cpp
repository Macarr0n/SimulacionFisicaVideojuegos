#include "Particles/Particle.h"
#include "ForceGenerators/ForceGenerator.h"
#include <cmath>

Particle::Particle(Vector3D pos, Vector3D vel, Vector3D acc, const ParticleProperties& props, bool visible)
	: m_pose(pos), m_renderItem(nullptr), m_vel(vel), m_acc(acc), m_mass(props.mass), m_damping(props.damping),
	m_lifeTime(props.lifeTime), m_integrator(props.integrator), m_radius(props.radius), m_color(props.color)
{
	if (visible) {
		physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(props.radius));
		m_renderItem = new RenderItem(shape, &m_pose, props.color);
	}
}

// copia todos los datos
Particle::Particle(const Particle& other)
	:m_pose(other.m_pose), m_renderItem(nullptr), m_vel(other.m_vel), m_acc(other.m_acc), m_force(),
	m_mass(other.m_mass), m_damping(other.m_damping), m_lifeTime(other.m_lifeTime),
	m_integrator(other.m_integrator), m_radius(other.m_radius), m_color(other.m_color)
{
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(m_radius));
	m_renderItem = new RenderItem(shape, &m_pose, m_color);
}

Particle::~Particle() {
	if (m_renderItem != nullptr) {
		m_renderItem->release();   // lo quita del render y lo borra
		m_renderItem = nullptr;
	}
}

void Particle::applyForces(const std::vector<ForceGenerator*>& forceGenerators, double t) {
	for (ForceGenerator* f : forceGenerators) {
		if (f->isActive()) {
			f->updateForce(this, t);
		}
	}
}

void Particle::integrate(double t) {
	float dt = static_cast<float>(t);
	m_lifeTime -= t;

	// a = F/m (Newton), más la aceleración fija que ya tuviera
	Vector3D totalAcc = m_acc + m_force * (1.0f / m_mass);

	switch (m_integrator) {
	case Integrator::EULER:
		// primero la posición, con la velocidad vieja
		m_pose.p += m_vel * dt;
		m_vel += totalAcc * dt;
		break;
	case Integrator::EULER_SEMI:
		// primero la velocidad, así la posición usa la nueva
		m_vel += totalAcc * dt;
		m_pose.p += m_vel * dt;
		break;
	}

	// damping^dt para que frene lo mismo vaya a los fps que vaya
	m_vel = m_vel * pow(m_damping, dt);

	// las fuerzas se vuelven a calcular cada frame, así que se vacían
	m_force = Vector3D();
}
