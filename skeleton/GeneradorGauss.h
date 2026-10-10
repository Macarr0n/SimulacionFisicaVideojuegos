#pragma once
#include "ParticleGenerator.h"

class GeneradorGauss : public ParticleGenerator
{
public:
	GeneradorGauss(physx::PxVec3 posMedia, physx::PxVec3 desvPos, physx::PxVec3 velMedia,
		physx::PxVec3 desvVel, int porFrame, double vida, Vector4 color, float radio)
		: m_posMedia(posMedia), m_desvPos(desvPos), m_velMedia(velMedia), m_desvVel(desvVel),
		m_porFrame(porFrame), m_vida(vida), m_color(color), m_radio(radio) {}

	void generateParticles(std::list<Particle*>& particles, const physx::PxVec3& gravedad) override {
		for (int i = 0; i < m_porFrame; i++) {
			physx::PxVec3 pos(gauss(m_posMedia.x, m_desvPos.x), gauss(m_posMedia.y, m_desvPos.y), gauss(m_posMedia.z, m_desvPos.z));
			physx::PxVec3 vel(gauss(m_velMedia.x, m_desvVel.x), gauss(m_velMedia.y, m_desvVel.y), gauss(m_velMedia.z, m_desvVel.z));
			particles.push_back(new Particle(pos, vel, gravedad, 0.99f, Integrator::EULER_SEMI, 1.0f, m_color, m_radio, m_vida));
		}
	}
private:
	physx::PxVec3 m_posMedia, m_desvPos, m_velMedia, m_desvVel;
	int m_porFrame;
	double m_vida;
	Vector4 m_color;
	float m_radio;
	std::normal_distribution<float> m_normal{ 0.f, 1.f };

	float gauss(float media, float desviacion) {
		return media + desviacion * m_normal(m_randomEngine);
	}
};
