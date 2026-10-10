#pragma once
#include "ParticleGenerator.h"

// distribución uniforme: todos los valores entre min y max igual de probables
class UniformGenerator : public ParticleGenerator
{
public:
	UniformGenerator(physx::PxVec3 minPos, physx::PxVec3 maxPos, physx::PxVec3 minVel,
		physx::PxVec3 maxVel, int perFrame, double lifeTime, Vector4 color, float radius)
		: m_minPos(minPos), m_maxPos(maxPos), m_minVel(minVel), m_maxVel(maxVel),
		m_perFrame(perFrame), m_lifeTime(lifeTime), m_color(color), m_radius(radius) {}

	void generateParticles(std::list<Particle*>& particles, const physx::PxVec3& gravity) override {
		for (int i = 0; i < m_perFrame; i++) {
			physx::PxVec3 pos(uniform(m_minPos.x, m_maxPos.x), uniform(m_minPos.y, m_maxPos.y), uniform(m_minPos.z, m_maxPos.z));
			physx::PxVec3 vel(uniform(m_minVel.x, m_maxVel.x), uniform(m_minVel.y, m_maxVel.y), uniform(m_minVel.z, m_maxVel.z));
			particles.push_back(new Particle(pos, vel, gravity, 0.99f, Integrator::EULER_SEMI, 1.0f, m_color, m_radius, m_lifeTime));
		}
	}

private:
	physx::PxVec3 m_minPos, m_maxPos;
	physx::PxVec3 m_minVel, m_maxVel;
	int m_perFrame;
	double m_lifeTime;
	Vector4 m_color;
	float m_radius;
	std::uniform_real_distribution<float> m_uniform{ 0.f, 1.f };   // valor aleatorio entre 0 y 1

	// pasa el valor de 0-1 al rango que queremos
	float uniform(float low, float high) {
		return low + (high - low) * m_uniform(m_randomEngine);
	}
};
