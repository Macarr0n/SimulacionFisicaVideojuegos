#pragma once
#include "Scenes/Scene.h"
#include "Particles/ParticleSystem.h"
#include "ForceGenerators/ExplosionForceGenerator.h"

class ExplosionScene : public Scene {
public:
    explicit ExplosionScene(std::string name) : Scene(std::move(name)) {}

    void init() override {
        // sin gravedad, para que las partículas esperen quietas a la explosión
        m_system = new ParticleSystem(Vector3D(), Vector3D(0.f, 10.f, 0.f), 100.f);

        // el sistema se queda la explosión y la borra; aquí solo guardamos el puntero para activarla
        m_explosion = new ExplosionForceGenerator(Vector3D(0.f, 10.f, 0.f), 30.f, 1000.f, 0.5f);
        m_system->addForceGenerator(m_explosion);

        const float spacing = 1.f;   // distancia entre partícula y partícula

        // cubo de 6x6x6 partículas alrededor del centro, alternando ligeras y pesadas
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                for (int k = 0; k < 6; k++) {
                    Vector3D pos((i - 2.5f) * spacing, 10.f + (j - 2.5f) * spacing, (k - 2.5f) * spacing);

                    // por defecto ligeras y amarillas
                    ParticleProperties props;
                    props.mass = 1.f;
                    props.color = Vector4(1.f, 1.f, 0.f, 1.f);
                    props.radius = 0.3f;
                    props.lifeTime = 1000.0;   // no mueren mientras esperan

                    // y una sí y otra no, pesadas y rojas (como un tablero de ajedrez)
                    if ((i + j + k) % 2 == 0) {
                        props.mass = 5.f;
                        props.color = Vector4(1.f, 0.f, 0.f, 1.f);
                    }

                    m_system->addParticle(new Particle(pos, Vector3D(), Vector3D(), props));
                }
            }
        }
    }

    void update(double dt) override {
        m_system->update(dt);
    }

    void keyPress(unsigned char key, const physx::PxTransform& camera) override {
        if (key == 'e' || key == 'E') {
            m_explosion->explode();
        }
    }

    void cleanup() override {
        delete m_system;          // también borra la explosión
        m_system = nullptr;
        m_explosion = nullptr;
    }

private:
    ParticleSystem* m_system{ nullptr };
    ExplosionForceGenerator* m_explosion{ nullptr };
};