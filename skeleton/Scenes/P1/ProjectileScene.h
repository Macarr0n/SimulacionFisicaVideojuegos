#pragma once
#include "Scenes/Scene.h"
#include "Particles/ParticleSystem.h"
#include "Particles/Projectile.h"
#include <iostream>

class ProjectileScene : public Scene {
public:
	explicit ProjectileScene(std::string name) : Scene(std::move(name)) {}

	void init() override {
		// solo guarda los proyectiles y los borra al salir de la zona, sin gravedad propia ya que la lleva cada pryectil
		m_projectiles = new ParticleSystem(Vector3D(), Vector3D(), 200.0f);

		// suelo y diana de referencia para apuntar
		m_groundTr = physx::PxTransform(physx::PxVec3(0.f, -0.5f, 0.f));
		m_ground = new RenderItem(CreateShape(physx::PxBoxGeometry(100.f, 0.5f, 100.f)), &m_groundTr, Vector4(0.5f, 0.5f, 0.5f, 1.f));

		m_targetTr = physx::PxTransform(physx::PxVec3(0.f, 15.f, 0.f));
		m_target = new RenderItem(CreateShape(physx::PxBoxGeometry(4.f, 4.f, 0.5f)), &m_targetTr, Vector4(1.f, 1.f, 0.f, 1.f));
	}

	void update(double dt) override {
		m_projectiles->update(dt);
	}

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {
		switch (key) {
		case 'f': case 'F':
			m_selected = &m_arrow;
			shoot(m_arrow);
			break;
		case 'b': case 'B':
			m_selected = &m_bolt;
			shoot(m_bolt);
			break;
		case '+':
			m_selected->realMass *= 1.1f;
			printInfo(*m_selected);
			break;
		case '-':
			m_selected->realMass /= 1.1f;
			printInfo(*m_selected);
			break;
		}
		
	}

	void cleanup() override {
		delete m_projectiles;
		m_projectiles = nullptr;
		m_ground->release();
		m_ground = nullptr;
		m_target->release();
		m_target = nullptr;
	}

private:
	ParticleSystem* m_projectiles{ nullptr };
	
	// masa real, velocidad real, velocidad simulada, gravedad real, color
	ProjectileType m_arrow{ 0.025f, 60.0f, 40.0f, 9.8f, Vector4(1.0f, 0.0f, 0.0f, 1.0f) };
	ProjectileType m_bolt{ 0.03f, 100.f, 40.f, 9.8f, Vector4(0.f, 0.f, 1.f, 1.f) };    // virote de ballesta (azul)
	ProjectileType* m_selected = &m_arrow;   // el último disparado, el que cambian + y -

	physx::PxTransform m_groundTr, m_targetTr;;
	RenderItem* m_ground{ nullptr };
	RenderItem* m_target{ nullptr };

	// sale un poco por delante de la cámara, hacia donde mira
	void shoot(const ProjectileType& type) {
		Camera* cam = GetCamera();
		Vector3D pos = cam->getEye() + cam->getDir() * 2.f; // Posición inicial de la flecha
		m_projectiles->addParticle(new Projectile(type, pos, cam->getDir()));
	}

	void printInfo(const ProjectileType& type) {
		std::cout << "Masa real: " << type.realMass << " kg | Masa simulada: " << Projectile::scaledMass(type)
			<< " kg | Gravedad simulada: " << Projectile::scaledGravity(type) << " m/s^2\n";
	}
};