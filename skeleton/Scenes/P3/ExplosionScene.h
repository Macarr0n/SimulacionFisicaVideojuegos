#pragma once
#include "Scenes/PhysicsScene.h"
#include "ForceGenerators/ExplosionForceGenerator.h"

class ExplosionScene : public PhysicsScene {
public:
    explicit ExplosionScene(std::string name) : PhysicsScene(std::move(name)) {}

    void init() override {
       // el sistema se queda la explosión y la borra; aquí solo guardamos el puntero para activarla
       m_explosion = new ExplosionForceGenerator(Vector3D(0.f, 10.f, 0.f), 30.f, 1000.f, 0.5f);
       addForce(m_explosion);

       // dos sistemas sin gravedad que comparten la misma explosión
       ParticleSystem* light = new ParticleSystem(Vector3D(), Vector3D(0.f, 10.f, 0.f), 100.f);
       ParticleSystem* heavy = new ParticleSystem(Vector3D(), Vector3D(0.f, 10.f, 0.f), 100.f);
       light->addForceGenerator(m_explosion);
       heavy->addForceGenerator(m_explosion);
       addSystem(light);
       addSystem(heavy);

        const float spacing = 1.f;   // distancia entre partícula y partícula

        // cubo de 6x6x6 partículas alrededor del centro, alternando ligeras y pesadas
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                for (int k = 0; k < 6; k++) {
                    Vector3D pos((i - 2.5f) * spacing, 10.f + (j - 2.5f) * spacing, (k - 2.5f) * spacing);

                    // por defecto ligeras y amarillas
                    ParticleProperties props;
                    props.radius = 0.3f;
                    props.lifeTime = 1000.0;   // no mueren mientras esperan

                    // y una sí y otra no, pesadas y rojas (como un tablero de ajedrez)
                    if ((i + j + k) % 2 == 0) {
                        props.mass = 5.f;
                        props.color = Vector4(1.f, 0.f, 0.f, 1.f);
                        heavy->addParticle(new Particle(pos, Vector3D(), Vector3D(), props));
                    }
                    // y ligeras y amarillas al ligero
                    else {
                        props.mass = 1.f;
                        props.color = Vector4(1.f, 1.f, 0.f, 1.f);
                        light->addParticle(new Particle(pos, Vector3D(), Vector3D(), props));
                    }
                }
            }
        }
    }

    void keyPress(unsigned char key, const physx::PxTransform& camera) override {
        if (key == 'e' || key == 'E') {
            m_explosion->explode();
        }
    }

private:
    // la escena la borra en cleanup, aquí solo se guarda para poder hacerla explotar
    ExplosionForceGenerator* m_explosion{ nullptr };
};