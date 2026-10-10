#pragma once
#include "ParticleGenerator.h"

// distribución normal (gaussiana), independiente en cada eje
class GaussianGenerator : public ParticleGenerator
{
public:
	GaussianGenerator(physx::PxVec3 meanPos, physx::PxVec3 posDev, physx::PxVec3 meanVel,
		physx::PxVec3 velDev, int perFrame, double lifeTime, Vector4 color, float radius)
		: m_meanPos(meanPos), m_posDev(posDev), m_meanVel(meanVel), m_velDev(velDev),
		m_perFrame(perFrame), m_lifeTime(lifeTime), m_color(color), m_radius(radius) {}

	void generateParticles(std::list<Particle*>& particles, const physx::PxVec3& gravity) override {
		for (int i = 0; i < m_perFrame; i++) {
			physx::PxVec3 pos(gauss(m_meanPos.x, m_posDev.x), gauss(m_meanPos.y, m_posDev.y), gauss(m_meanPos.z, m_posDev.z));
			physx::PxVec3 vel(gauss(m_meanVel.x, m_velDev.x), gauss(m_meanVel.y, m_velDev.y), gauss(m_meanVel.z, m_velDev.z));
			particles.push_back(new Particle(pos, vel, gravity, 0.99f, Integrator::EULER_SEMI, 1.0f, m_color, m_radius, m_lifeTime));
		}
	}

private:
	physx::PxVec3 m_meanPos, m_posDev;   // posición media y su desviación típica
	physx::PxVec3 m_meanVel, m_velDev;   // velocidad media y su desviación típica
	int m_perFrame;
	double m_lifeTime;
	Vector4 m_color;
	float m_radius;
	std::normal_distribution<float> m_normal{ 0.f, 1.f };   // normal estándar: media 0, desviación 1

	// pasa la normal estándar a la media y desviación que queremos
	// (con desviación 0 siempre devuelve la media)
	float gauss(float mean, float dev) {
		return mean + dev * m_normal(m_randomEngine);
	}
};
