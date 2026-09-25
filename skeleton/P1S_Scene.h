#pragma once
#include "Scene.h"
#include "RenderUtils.hpp"
#include <array>
#include "Vector3D.h"
#include "Particle.h"

class P1_Scene :
    public Scene
{
private:
	Particle* m_particle { nullptr };
public:
    explicit P1_Scene(std::string name) : Scene(std::move(name)) {}

    void init() override {
		m_particle = new Particle(Vector3D(0.f, 0.f, 0.f), Vector3D(10.f, 10.f, 0.f), Vector3D(0.f, -9.8f, 0.f), 0.5f, Integrador::EULER_SEMI);
    }

    void update(double dt) override {
        // Lógica/Integración del alumno (por ejemplo, movimiento simple)
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

