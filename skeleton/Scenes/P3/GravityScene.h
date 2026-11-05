#pragma once
#include "Scenes/PhysicsScene.h"
#include "ParticleGenerators/GaussianGenerator.h"
#include "ForceGenerators/GravityForceGenerator.h"

class GravityScene : public PhysicsScene {
public:
    explicit GravityScene(std::string name) :PhysicsScene(std::move(name)) {}

    void init() override {
        // las fuerzas se crean aparte y luego se le dan a los sistemas que queramos
        m_gravity = new GravityForceGenerator(Vector3D(0.f, -10.f, 0.f));
        m_weakGravity = new GravityForceGenerator(Vector3D(0.f, -5.f, 0.f));
        addForce(m_gravity);
        addForce(m_weakGravity);

        // Izquierda: gravedad como aceleración fija, como en la práctica 2
        ParticleSystem* accSystem = new ParticleSystem(Vector3D(0.f, -10.f, 0.f), Vector3D(0.f, 0.f, 0.f), 100.f);
        accSystem->addGenerator(fountain(Vector3D(-20.f, 0.f, 0.f), Vector4(0.2f, 0.6f, 1.f, 1.f), 1.f));   // azul
        addSystem(accSystem);

        // Centro: gravedad como fuerza, con partículas de masas muy distintas
        ParticleSystem* forceSystem = new ParticleSystem(Vector3D(0.f, 0.f, 0.f), Vector3D(0.f, 0.f, 0.f), 100.f);   // sin aceleración fija
        forceSystem->addForceGenerator(m_gravity);
        forceSystem->addGenerator(fountain(Vector3D(0.f, 0.f, 0.f), Vector4(1.f, 1.f, 0.f, 1.f), 1.f));     // amarillas, masa 1
        forceSystem->addGenerator(fountain(Vector3D(0.f, 0.f, 0.f), Vector4(1.f, 0.f, 0.f, 1.f), 100.f));   // rojas, masa 100
        addSystem(forceSystem);

        // Derecha: la segunda gravedad, mucho más débil
        ParticleSystem* weakSystem = new ParticleSystem(Vector3D(0.f, 0.f, 0.f), Vector3D(0.f, 0.f, 0.f), 100.f);
        weakSystem->addForceGenerator(m_weakGravity);
        weakSystem->addGenerator(fountain(Vector3D(20.f, 0.f, 0.f), Vector4(0.f, 1.f, 0.f, 1.f), 1.f));     // verdes
        addSystem(weakSystem);
    }

    // G gravedad normal, H gravedad débil
    void keyPress(unsigned char key, const physx::PxTransform& camera) override {
        switch (key) {
        case 'g': case 'G':
            toggleForce(m_gravity, "Gravedad");
            break;
        case 'h': case 'H':
            toggleForce(m_weakGravity, "Gravedad debil");
            break;
        }
    }

private:
    // la escena los borra en cleanup, aquí solo se guardan para poder encenderlos y apagarlos
    ForceGenerator* m_gravity{ nullptr };
    ForceGenerator* m_weakGravity{ nullptr };

    // todas las fuentes son iguales, solo cambia dónde están, el color y la masa
    GaussianGenerator* fountain(physx::PxVec3 pos, Vector4 color, float mass) {
        ParticleProperties props;
        props.mass = mass;
        props.color = color;
        props.radius = 0.3f;
        props.lifeTime = 3.0;
        return new GaussianGenerator(pos, Vector3D(), Vector3D(0.f, 15.f, 0.f), Vector3D(1.f, 1.f, 1.f), 1, props);
    }
};