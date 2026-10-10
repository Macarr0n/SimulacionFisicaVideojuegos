#pragma once
#include "Scene.h"
#include "ParticleSystem.h"
#include "GaussianGenerator.h"
#include "UniformGenerator.h"

class P2_Scene : public Scene {
public:
    explicit P2_Scene(std::string name) : Scene(std::move(name)) {}

    void init() override {
        m_system = new ParticleSystem(physx::PxVec3(0.f, -10.f, 0.f),   // gravedad
            physx::PxVec3(0.f, 0.f, 0.f),     // centro de la zona de acción
            100.f);                           // radio de la zona de acción

        // Fuente (gaussiana): sale siempre del mismo punto, hacia arriba y abriéndose un poco
        m_system->addGenerator(new GaussianGenerator(
            physx::PxVec3(0.f, 0.f, 0.f),      // posición media
            physx::PxVec3(0.f, 0.f, 0.f),      // desviación de la posición
            physx::PxVec3(0.f, 20.f, 0.f),     // velocidad media, hacia arriba
            physx::PxVec3(2.f, 2.f, 2.f),      // desviación de la velocidad
            2,                                 // partículas por frame
            4.0,                               // tiempo de vida: 2s subiendo y 2s bajando
            Vector4(0.2f, 0.6f, 1.f, 1.f),     // azul
            0.3f));                            // radio

        // Lluvia (uniforme): gotas repartidas por un cuadrado en el cielo que caen
        m_system->addGenerator(new UniformGenerator(
            physx::PxVec3(-30.f, 30.f, 10.f),  // posición mínima
            physx::PxVec3(-10.f, 30.f, 30.f),  // posición máxima
            physx::PxVec3(0.f, -5.f, 0.f),     // velocidad mínima
            physx::PxVec3(0.f, -2.f, 0.f),     // velocidad máxima
            2,                                 // partículas por frame
            2.0,                               // tiempo de vida: más o menos lo que tardan en llegar al suelo
            Vector4(0.6f, 0.7f, 1.f, 1.f),     // azul claro
            0.15f));                           // radio

        // Niebla (gaussiana): posiciones muy repartidas, velocidades pequeñas, partículas pequeñas y transparentes
        m_system->addGenerator(new GaussianGenerator(
            physx::PxVec3(20.f, 2.f, -20.f),   // posición media
            physx::PxVec3(6.f, 1.f, 6.f),      // desviación de la posición, grande
            physx::PxVec3(0.f, 5.f, 0.f),      // sube un poco para que la gravedad la devuelva a la misma altura
            physx::PxVec3(1.f, 1.f, 1.f),      // desviación de la velocidad, pequeña
            3,                                 // partículas por frame
            1.0,                               // tiempo de vida: muere justo al volver abajo, así parece que flota
            Vector4(0.9f, 0.9f, 0.9f, 0.3f),   // gris muy claro y transparente
            0.1f));                            // radio
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
