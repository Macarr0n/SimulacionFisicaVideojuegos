#pragma once
#include "Scenes/PhysicsScene.h"
#include "ParticleGenerators/GaussianGenerator.h"
#include "ForceGenerators/GravityForceGenerator.h"
#include "ForceGenerators/WhirlwindForceGenerator.h"

class WhirlwindScene : public PhysicsScene {
public:
	explicit WhirlwindScene(std::string name) : PhysicsScene(std::move(name)) {}

	void init() override {
		m_gravity = (new GravityForceGenerator(Vector3D(0.f, -10.f, 0.f)));

		// torbellino en el origen, radio 30, K = 0.5 y k1 = 1
		m_whirlwind = (new WhirlwindForceGenerator(Vector3D(0.f, 0.f, 0.f), 30.f, 1.5f, 1.f));
		addForce(m_gravity);
		addForce(m_whirlwind);

		ParticleSystem* system = new ParticleSystem(Vector3D(), Vector3D(0.f, 0.f, 0.f), 100.f);
		system->addForceGenerator(m_gravity);
		system->addForceGenerator(m_whirlwind);

		// salen quietas del suelo alrededor del centro
		system->addGenerator(groundParticles(Vector4(1.f, 1.f, 0.f, 1.f), 1.f));   // amarillas, masa 1
		system->addGenerator(groundParticles(Vector4(1.f, 0.f, 0.f, 1.f), 5.f));   // rojas, masa 5
		addSystem(system);
	}

	// G gravedad, K torbellino
	void keyPress(unsigned char key, const physx::PxTransform& camera) override {
		switch (key) {
		case 'g': case 'G':
			toggleForce(m_gravity, "Gravedad");
			break;
		case 'k': case 'K':
			toggleForce(m_whirlwind, "Torbellino");
			break;
		}
	}

private:
	ForceGenerator* m_gravity{ nullptr };
	ForceGenerator* m_whirlwind{ nullptr };

	GaussianGenerator* groundParticles(Vector4 color, float mass) {
		ParticleProperties props;
		props.mass = mass;
		props.color = color;
		props.radius = 0.3f;
		props.lifeTime = 8.0;
		return new GaussianGenerator(Vector3D(0.f, 0.f, 0.f), Vector3D(5.f, 0.f, 5.f), Vector3D(), Vector3D(),1, props);
	}
};
