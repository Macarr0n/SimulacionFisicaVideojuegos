#pragma once
#include "Scene.h"
#include "RenderUtils.hpp"
#include <array>
#include "Vector3D.h"

class P1_Scene :
    public Scene
{
public:
    explicit P1_Scene(std::string name) : Scene(std::move(name)) {}

    void init() override {

    }

    void update(double dt) override {
        // Lógica/Integración del alumno (por ejemplo, movimiento simple)
    }

    void keyPress(unsigned char key, const physx::PxTransform& camera) override {

    }

    void cleanup() override {

    }

private:

};

