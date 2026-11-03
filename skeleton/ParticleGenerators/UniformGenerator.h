#pragma once
#include "ParticleGenerators/ParticleGenerator.h"

// distribución uniforme: todos los valores entre min y max igual de probables
class UniformGenerator : public ParticleGenerator
{
public:
	UniformGenerator(Vector3D minPos, Vector3D maxPos, Vector3D minVel, Vector3D maxVel,
		int perFrame, const ParticleProperties& props)
		: ParticleGenerator(perFrame, props),
		m_minPos(minPos), m_maxPos(maxPos), m_minVel(minVel), m_maxVel(maxVel) {}

protected:
	Vector3D randomPos() override { return uniform(m_minPos, m_maxPos); }
	Vector3D randomVel() override { return uniform(m_minVel, m_maxVel); }

private:
	Vector3D m_minPos, m_maxPos;
	Vector3D m_minVel, m_maxVel;
	std::uniform_real_distribution<float> m_uniform{ 0.f, 1.f };   // valor aleatorio entre 0 y 1

	// pasa el valor de 0-1 al rango de cada eje
	Vector3D uniform(const Vector3D& low, const Vector3D& high) {
		return Vector3D(low.x + (high.x - low.x) * m_uniform(m_randomEngine),
			low.y + (high.y - low.y) * m_uniform(m_randomEngine),
			low.z + (high.z - low.z) * m_uniform(m_randomEngine));
	}
};
