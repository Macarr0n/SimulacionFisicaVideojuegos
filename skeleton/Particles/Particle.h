#pragma once
#include "Math/Vector3D.h"
#include "RenderUtils.hpp"
#include <vector>

class ForceGenerator;   

// cómo se mueve la partícula en cada frame
enum class Integrator {
	EULER,
	EULER_SEMI
};

// todo esto es lo que defina a una particula aparte de su posición, velocidad y aceleración
struct ParticleProperties {
	float mass = 1.f;
	float damping = 0.99f;		// entre 0 y 1, la va frenando poco a poco
	double lifeTime = 5.0;		// segundos de vida 
	float radius = 1.f;
	Vector4 color = Vector4(1.f, 1.f, 1.f, 1.f);
	Integrator integrator = Integrator::EULER_SEMI;
};

class Particle
{
public:
	// visible igual a falso para que la partícula modelo de los generadores, que no se pinta
	Particle(Vector3D pos, Vector3D vel, Vector3D acc, const ParticleProperties& props = ParticleProperties(), bool visible = true);
	~Particle();

	// la copia crea su propio render item, que apunta a m_pose
	Particle(const Particle& other);
	Particle& operator=(const Particle&) = delete;

	void integrate(double t);
	void addForce(const Vector3D& f) { m_force += f; }

	// la partícula llama a cada generador de fuerzas pasándose a sí misma
	void applyForces(const std::vector<ForceGenerator*>& forceGenerators, double t);

	Vector3D getPos() const { return m_pose.p; }
	Vector3D getVel() const { return m_vel; }
	float getMass() const { return m_mass; }
	bool isAlive() const { return m_lifeTime > 0.0; }

	void setPos(const Vector3D& pos) { m_pose.p = pos; }
	void setVel(const Vector3D& vel) { m_vel = vel; }
	void setAcc(const Vector3D& acc) { m_acc = acc; }

private:
	physx::PxTransform m_pose;	// el render item lee la posición de aquí
	RenderItem* m_renderItem;
	Vector3D m_vel;				
	Vector3D m_acc;				// aceleración fija
	Vector3D m_force;			// suma de las fuerzas de este frame
	float m_damping;            
	float m_mass;
	double m_lifeTime;          // segundos que le quedan antes de morir
	Integrator m_integrator;
	float m_radius;				// se guardan para que las copias se pinten igual
	Vector4 m_color;
};
