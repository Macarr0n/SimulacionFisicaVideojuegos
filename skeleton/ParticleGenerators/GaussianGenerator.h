#pragma once
#include "ParticleGenerators/ParticleGenerator.h"

// distribución normal (gaussiana), independiente en cada eje
class GaussianGenerator : public ParticleGenerator
{
public:
	GaussianGenerator(Vector3D meanPos, Vector3D posDev, Vector3D meanVel, Vector3D velDev,
		int perFrame, const ParticleProperties& props)
		: ParticleGenerator(perFrame, props), 
		m_meanPos(meanPos), m_posDev(posDev), m_meanVel(meanVel), m_velDev(velDev) {}

protected:
	Vector3D randomPos() override { return gauss(m_meanPos, m_posDev); }
	Vector3D randomVel() override { return gauss(m_meanVel, m_velDev); }

private:
	Vector3D m_meanPos, m_posDev;   // posición media y su desviación típica
	Vector3D m_meanVel, m_velDev;   // velocidad media y su desviación típica
	std::normal_distribution<float> m_normal{ 0.f, 1.f };   // normal estándar: media 0, desviación 1

	// pasa la normal estándar a la media y desviación que queremos
	// (con desviación 0 siempre devuelve la media)
	Vector3D gauss(const Vector3D& mean, const Vector3D& dev) {
		return Vector3D(mean.x + dev.x * m_normal(m_randomEngine),
			mean.y + dev.y * m_normal(m_randomEngine),
			mean.z + dev.z * m_normal(m_randomEngine));
	}
};
