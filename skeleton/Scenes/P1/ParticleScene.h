#pragma once
#include "Scenes/Scene.h"
#include "RenderUtils.hpp"
#include <array>
#include "Math/Vector3D.h"
#include "Particles/Particle.h"

class ParticleScene :
    public Scene
{
private:
	Particle* m_particle { nullptr };
public:
    explicit ParticleScene(std::string name) : Scene(std::move(name)) {}

    void init() override {
        ParticleProperties props;
        props.damping = 0.5f;
        props.integrator = Integrator::EULER_SEMI;

        // sale en diagonal hacia arriba y la gravedad la hace caer en parábola
        m_particle = new Particle(Vector3D(0.f, 0.f, 0.f), Vector3D(10.f, 10.f, 0.f), Vector3D(0.f, -9.8f, 0.f), props);
    }

    void update(double dt) override {
        if (m_particle) {
            m_particle->integrate(dt);
        }
    }

    void keyPress(unsigned char key, const physx::PxTransform& camera) override {

    }

    void cleanup() override {
        if (m_particle) {
            delete m_particle;
            m_particle = nullptr;
        }
    }
};

