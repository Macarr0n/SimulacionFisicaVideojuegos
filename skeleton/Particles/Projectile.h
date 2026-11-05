#pragma once
#include "Particles/Particle.h"

// datos reales de un tipo de proyectil y la velocidad a la que lo simulamos
struct ProjectileType {
	float realMass;			// kg
	float realVel;			// m/s
	float simVel;			// m/s, más lenta para poder verlo
	float gravedadReal = 9.8f;
	Vector4 color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
};

// proyectil, una partícula con la velocidad, la masa y la gravedad escaladas
class Projectile : public Particle {
public:
	Projectile(const ProjectileType& type, Vector3D pos, Vector3D dir) 
	: Particle(pos, dir * type.simVel, Vector3D(0.f, -scaledGravity(type), 0.0f), scaledProps(type)) {}

	// misma energía cinética: m_sim = m_real * (v_real / v_sim)²
	static float scaledMass(const ProjectileType& type) {
		float r = type.realVel / type.simVel;
		return type.realMass * r * r;
	}

	// mismo arco: g_sim = g_real * (v_sim / v_real)²
	static float scaledGravity(const ProjectileType& type) {
		float r = type.simVel / type.realVel;
		return type.gravedadReal * r * r;
	}

private:
	static ParticleProperties scaledProps(const ProjectileType& type) {
		ParticleProperties props;
		props.mass = scaledMass(type);
		props.color = type.color;
		props.lifeTime = 10.0;
		return props;
	}
};