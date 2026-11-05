#pragma once
#include "Scenes/Scene.h"
#include "Particles/ParticleSystem.h"
#include <vector>
#include <iostream>

// escena con sistemas de partículas y generadores de fuerzas
class PhysicsScene : public Scene
{
public:
	explicit PhysicsScene(std::string name) : Scene(std::move(name)) {}

	void update(double dt) override {
		// las fuerzas que cambian con el tiempo avanzan una sola vez por frame, aunque las usen varios sistemas
		for (ForceGenerator* f : m_forces) {
			f->update(dt);
		}
		for (ParticleSystem* s : m_systems) {
			s->update(dt);
		}
	}

	void cleanup() override {
		for (ParticleSystem* s : m_systems) delete s;
		m_systems.clear();
		for (ForceGenerator* f : m_forces) delete f;
		m_forces.clear();
	}

protected:
	// la escena se queda con ellos y los borra en cleanup
	void addSystem(ParticleSystem* system) { m_systems.push_back(system); }
	void addForce(ForceGenerator* force) { m_forces.push_back(force); }

	// enciende o apaga una fuerza y lo dice por consola
	void toggleForce(ForceGenerator* force, const std::string& name) {
		force->setActive(!force->isActive());
		if (force->isActive()) std::cout << name << ": ON\n";
		else std::cout << name << ": OFF\n";
	}

private:
	std::vector<ParticleSystem*> m_systems;
	std::vector<ForceGenerator*> m_forces;
};