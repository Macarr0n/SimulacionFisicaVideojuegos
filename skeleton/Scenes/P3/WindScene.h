#pragma once
#include "Scenes/Scene.h"
#include "Particles/ParticleSystem.h"
#include "ParticleGenerators/GaussianGenerator.h"
#include "ForceGenerators/GravityForceGenerator.h"
#include "ForceGenerators/WindForceGenerator.h"

class WindScene : public Scene {
public:
	explicit WindScene(std::string name) : Scene(std::move(name)) {}

	void init() override {
		m_system = new ParticleSystem(Vector3D(), Vector3D(0.f, 0.f, 0.f), 100.f);
		m_system->addForceGenerator(new GravityForceGenerator(Vector3D(0.f, -10.f, 0.f)));

		// viento hacia la derecha en una esfera de radio 10 a media altura
		m_system->addForceGenerator(new WindForceGenerator(Vector3D(200.f, 0.f, 0.f), Vector3D(0.f, 12.f, 0.f), 10.f, 1.f));

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
		ParticleProperties props;
		props.mass = mass;
		props.color = color;
		props.radius = 0.3f;
		props.lifeTime = 5.0;
		return new GaussianGenerator(Vector3D(0.f, 30.f, 0.f), Vector3D(2.f, 0.f, 2.f), Vector3D(), Vector3D(), 1, props);
	}
};