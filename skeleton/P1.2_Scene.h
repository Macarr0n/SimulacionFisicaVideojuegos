#pragma once
#include "Scene.h"
#include "RenderUtils.hpp"
#include "Particle.h"
#include "Proyectil.h"
#include <vector>
#include <iostream>

class P1_2_Scene : public Scene {
public:
	explicit P1_2_Scene(std::string name) : Scene(std::move(name)) {}

	void init() override {
		m_sueloTr = physx::PxTransform(physx::PxVec3(0.f, -0.5f, 0.f));
		m_suelo = new RenderItem(CreateShape(physx::PxBoxGeometry(100.f, 0.5f, 100.f)), &m_sueloTr, Vector4(0.5f, 0.5f, 0.5f, 1.f));

		m_dianaTr = physx::PxTransform(physx::PxVec3(0.f, 15.f, 0.f));
		m_diana = new RenderItem(CreateShape(physx::PxBoxGeometry(4.f, 4.f, 0.5f)), &m_dianaTr, Vector4(1.f, 1.f, 0.f, 1.f));
	}

	void update(double dt) override {
		for (Particle* p : m_proyectiles) {
			p->integrate(dt);
		}
	}

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {
		switch (key) {
		case 'f': case 'F':
			disparar(m_flecha);
			break;
		case '+':
			m_flecha.masaReal *= 1.1f;
			break;
		case '-':
			m_flecha.masaReal /= 1.1f;
			break;
		}
		
	}

	void cleanup() override {
		for (Particle* p : m_proyectiles) {
			delete p;
		}
		m_proyectiles.clear();

		if (m_suelo) {
			m_suelo->release();
			m_suelo = nullptr;
		}
		if (m_diana) {
			m_diana->release();
			m_diana = nullptr;
		}
	}

private:
	std::vector<Particle*> m_proyectiles;
	TipoProyectil m_flecha{ 0.025f, 60.f, 40.f };

	physx::PxTransform m_sueloTr;
	physx::PxTransform m_dianaTr;
	RenderItem* m_suelo{ nullptr };
	RenderItem* m_diana{ nullptr };

	void disparar(const TipoProyectil& tipo) {
		Camera* cam = GetCamera();

		Vector3D pos = cam->getEye() + cam->getDir() * 2.f; // Posición inicial de la flecha
		Vector3D vel = cam->getDir() * tipo.velSim;		    // Velocidad inicial de la flecha
		Vector3D ace(0.f, -tipo.gravedadSim(), 0.f);	    // Gravedad simulada hacia abajo
		
		m_proyectiles.push_back(new Particle(pos, vel, ace, 0.99f, Integrator::EULER_SEMI, tipo.masaSim()));
	}
};