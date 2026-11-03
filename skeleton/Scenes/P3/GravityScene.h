#pragma once
#include "Scenes/Scene.h"
#include "Particles/ParticleSystem.h"
#include "ParticleGenerators/GaussianGenerator.h"
#include "ForceGenerators/GravityForceGenerator.h"
#include <vector>

class GravityScene : public Scene {
public:
    explicit GravityScene(std::string name) : Scene(std::move(name)) {}

    void init() override {
        // Izquierda: gravedad como aceleración fija, como en la práctica 2
        ParticleSystem* accSystem = new ParticleSystem(Vector3D(0.f, -10.f, 0.f), Vector3D(0.f, 0.f, 0.f), 100.f);
        accSystem->addGenerator(fountain(Vector3D(-20.f, 0.f, 0.f), Vector4(0.2f, 0.6f, 1.f, 1.f), 1.f));   // azul
        m_systems.push_back(accSystem);

        // Centro: gravedad como fuerza, con partículas de masas muy distintas
        ParticleSystem* forceSystem = new ParticleSystem(Vector3D(0.f, 0.f, 0.f), Vector3D(0.f, 0.f, 0.f), 100.f);   // sin aceleración fija
        forceSystem->addForceGenerator(new GravityForceGenerator(Vector3D(0.f, -10.f, 0.f)));
        forceSystem->addGenerator(fountain(Vector3D(0.f, 0.f, 0.f), Vector4(1.f, 1.f, 0.f, 1.f), 1.f));     // amarillas, masa 1
        forceSystem->addGenerator(fountain(Vector3D(0.f, 0.f, 0.f), Vector4(1.f, 0.f, 0.f, 1.f), 100.f));   // rojas, masa 100
        m_systems.push_back(forceSystem);

        // Derecha: la segunda gravedad, mucho más débil
        ParticleSystem* weakSystem = new ParticleSystem(Vector3D(0.f, 0.f, 0.f), Vector3D(0.f, 0.f, 0.f), 100.f);
        weakSystem->addForceGenerator(new GravityForceGenerator(Vector3D(0.f, -1.f, 0.f)));
        weakSystem->addGenerator(fountain(Vector3D(20.f, 0.f, 0.f), Vector4(0.f, 1.f, 0.f, 1.f), 1.f));     // verdes
        m_systems.push_back(weakSystem);
    }

    void update(double dt) override {
        for (ParticleSystem* s : m_systems) s->update(dt);
    }

    void cleanup() override {
        for (ParticleSystem* s : m_systems) delete s;
        m_systems.clear();
    }

private:
    std::vector<ParticleSystem*> m_systems;

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