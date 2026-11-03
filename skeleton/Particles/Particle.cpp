#include "Particles/Particle.h"
#include <cmath>

Particle::Particle(Vector3D pos, Vector3D vel, Vector3D acc, const ParticleProperties& props)
	: m_pose(pos), m_vel(vel), m_acc(acc), m_mass(props.mass), m_damping(props.damping),
	m_lifeTime(props.lifeTime), m_integrator(props.integrator)
{
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(props.radius));
	m_renderItem = new RenderItem(shape, &m_pose, props.color);
}

Particle::~Particle() {
	m_renderItem->release();   // lo quita del render y lo borra
	m_renderItem = nullptr;
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
