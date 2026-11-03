#pragma once
#include "Scenes/Scene.h"
#include "Particles/ParticleSystem.h"
#include "ParticleGenerators/GaussianGenerator.h"
#include "ParticleGenerators/UniformGenerator.h"

class EmittersScene : public Scene {
public:
    explicit EmittersScene(std::string name) : Scene(std::move(name)) {}

    void init() override {
        // gravedad en -10 en Y y zona de acción de radio alrededor del origen
        m_system = new ParticleSystem(physx::PxVec3(0.f, -10.f, 0.f),   // gravedad
            Vector3D(0.f, 0.f, 0.f),     // centro de la zona de acción
            100.f);                      // radio de la zona de acción

        // Fuente gaussiana, sale siempre del mismo punto, hacia arriba y abriéndose un poco
        ParticleProperties water;
        water.lifeTime = 4.0;
        water.radius = 0.3f;
        water.color = Vector4(0.2f, 0.6f, 1.0f, 1.0f);
        m_system->addGenerator(new GaussianGenerator(
            Vector3D(0.f, 0.f, 0.f),      // posición media
            Vector3D(0.f, 0.f, 0.f),      // desviación de la posición
            Vector3D(0.f, 20.f, 0.f),     // velocidad media, hacia arriba
            Vector3D(2.f, 2.f, 2.f),      // desviación de la velocidad
            2,                            // partículas por frame
            water));

        // Lluvia uniforme, gotas repartidas por un cuadrado en el cielo que caen
        ParticleProperties rain;
        rain.lifeTime = 2.0;              // más o menos lo que tardan en llegar al suelo
        rain.radius = 0.15f;
        rain.color = Vector4(0.6f, 0.7f, 1.f, 1.f);
        m_system->addGenerator(new UniformGenerator(
            Vector3D(-30.f, 30.f, 10.f),  // posición mínima
            Vector3D(-10.f, 30.f, 30.f),  // posición máxima
            Vector3D(0.f, -5.f, 0.f),     // velocidad mínima
            Vector3D(0.f, -2.f, 0.f),     // velocidad máxima
            2,                            // partículas por frame
            rain));

        // Niebla gaussiana, posiciones muy repartidas, velocidades pequeñas, partículas pequeñas y transparentes
        ParticleProperties fog;
        fog.lifeTime = 1.0;                     // muere justo al volver abajo, así parece que flota
        fog.radius = 0.1f;
        fog.color = Vector4(0.9f, 0.9f, 0.9f, 0.3f);    // alfa 0.3, transparente
        m_system->addGenerator(new GaussianGenerator(
            Vector3D(20.f, 2.f, -20.f),   // posición media
            Vector3D(6.f, 1.f, 6.f),      // desviación de la posición, grande
            Vector3D(0.f, 5.f, 0.f),      // sube un poco para que la gravedad la devuelva a la misma altura
            Vector3D(1.f, 1.f, 1.f),      // desviación de la velocidad, pequeña
            3,                            // partículas por frame
            fog));                            
    }

    void update(double dt) override {
        m_system->update(dt);
    }

    void cleanup() override {
        delete m_system;   // borra también los generadores y las partículas
        m_system = nullptr;
    }

private:
    ParticleSystem* m_system{ nullptr };
};
