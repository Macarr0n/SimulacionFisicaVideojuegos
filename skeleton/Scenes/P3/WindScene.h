#pragma once
#include "Scenes/PhysicsScene.h"
#include "ParticleGenerators/GaussianGenerator.h"
#include "ForceGenerators/GravityForceGenerator.h"
#include "ForceGenerators/WindForceGenerator.h"

class WindScene : public PhysicsScene {
public:
	explicit WindScene(std::string name) : PhysicsScene(std::move(name)) {}

	void init() override {
		m_gravity = new GravityForceGenerator(Vector3D(0.f, -10.f, 0.f));
		// viento hacia la derecha en una esfera de radio 10 a media altura
		m_wind = (new WindForceGenerator(Vector3D(200.f, 0.f, 0.f), Vector3D(0.f, 12.f, 0.f), 10.f, 1.f));
		addForce(m_gravity);
		addForce(m_wind);

		ParticleSystem* system = new ParticleSystem(Vector3D(), Vector3D(0.f, 0.f, 0.f), 100.f);
		system->addForceGenerator(m_gravity);
		system->addForceGenerator(m_wind);

		// caen desde arriba y atraviesan el viento
		system->addGenerator(drops(Vector4(1.f, 1.f, 0.f, 1.f), 1.f));	// amarillas, masa 1
		system->addGenerator(drops(Vector4(1.f, 0.f, 0.f, 1.f), 20.f));   // rojas, masa 20
		addSystem(system);

	}

	// H gravedad, G viento
	void keyPress(unsigned char key, const physx::PxTransform& camera) override {
		switch (key) {
		case 'h': case 'H':
			toggleForce(m_gravity, "Gravedad");
			break;
		case 'g': case 'G':
			toggleForce(m_wind, "Viento");
			break;
		}
	}

private:
	ForceGenerator* m_gravity{ nullptr };
	ForceGenerator* m_wind{ nullptr };

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