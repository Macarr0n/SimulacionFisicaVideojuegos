#pragma once
#include "Scenes/Scene.h"
#include "Particles/ParticleSystem.h"
#include "ParticleGenerators/GaussianGenerator.h"
#include "ForceGenerators/GravityForceGenerator.h"
#include "ForceGenerators/WhirlwindForceGenerator.h"

class WhirlwindScene : public Scene {
public:
	explicit WhirlwindScene(std::string name) : Scene(std::move(name)) {}

	void init() override {
		m_system = new ParticleSystem(physx::PxVec3(0.f, 0.f, 0.f), physx::PxVec3(0.f, 0.f, 0.f), 100.f);
		m_system->addForceGenerator(new GravityForceGenerator(Vector3D(0.f, -10.f, 0.f)));

		// torbellino en el origen, radio 30, K = 0.5 y k1 = 1
		m_system->addForceGenerator(new WhirlwindForceGenerator(physx::PxVec3(0.f, 0.f, 0.f), 30.f, 1.5f, 1.f));

		// salen quietas del suelo alrededor del centro
		m_system->addGenerator(groundParticles(Vector4(1.f, 1.f, 0.f, 1.f), 1.f));   // amarillas, masa 1
		m_system->addGenerator(groundParticles(Vector4(1.f, 0.f, 0.f, 1.f), 5.f));   // rojas, masa 5
	}

	void update(double dt) override {
		m_system->update(dt);
	}

	void cleanup() override {
		delete m_system;
		m_system = nullptr;
	}

private:
	ParticleSystem* m_system{ nullptr };

	GaussianGenerator* groundParticles(Vector4 color, float mass) {
		return new GaussianGenerator(physx::PxVec3(0.f, 0.f, 0.f), physx::PxVec3(5.f, 0.f, 5.f),
			physx::PxVec3(0.f, 0.f, 0.f), physx::PxVec3(0.f, 0.f, 0.f),
			1, 8.0, color, 0.3f, mass);
	}
};
