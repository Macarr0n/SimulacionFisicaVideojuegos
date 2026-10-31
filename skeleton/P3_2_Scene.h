#pragma once
#include "Scene.h"
#include "ParticleSystem.h"
#include "GaussianGenerator.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"

class P3_2_Scene : public Scene {
public:
	explicit P3_2_Scene(std::string name) : Scene(std::move(name)) {}

	void init() override {
		m_system = new ParticleSystem(physx::PxVec3(0.f, 0.f, 0.f), physx::PxVec3(0.f, 0.f, 0.f), 100.f);
		m_system->addForceGenerator(new GravityForceGenerator(Vector3D(0.f, -10.f, 0.f)));

		// viento hacia la derecha en una esfera de radio 10 aa media altura
		m_system->addForceGenerator(new WindForceGenerator(Vector3D(200.f, 0.f, 0.f), physx::PxVec3(0.f, 12.f, 0.f), 10.f, 1.f));

		// caen desde arriba y atraviesan el viento
		m_system->addGenerator(drops(Vector4(1.f, 1.f, 0.f, 1.f), 1.f));	// amarillas, masa 1
		m_system->addGenerator(drops(Vector4(1.f, 0.f, 0.f, 1.f), 20.f));   // rojas, masa 20

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

	// gotas que salen quietas de una zona pequeña a y = 30
	GaussianGenerator* drops(Vector4 color, float mass) {
		return new GaussianGenerator(physx::PxVec3(0.f, 30.f, 0.f), physx::PxVec3(2.f, 0.f, 2.f),
			physx::PxVec3(0.f, 0.f, 0.f), physx::PxVec3(0.f, 0.f, 0.f),
			1, 5.0, color, 0.3f, mass);
	}
};