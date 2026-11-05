#pragma once
#include "Scenes/PhysicsScene.h"
#include "Scenes/Game/Strand.h"
#include "ForceGenerators/GravityForceGenerator.h"
#include "ForceGenerators/GroundForceGenerator.h"
#include "ForceGenerators/SpringForceGenerator.h"
#include "ForceGenerators/AnchoredSpringForceGenerator.h"
#include "ForceGenerators/WindForceGenerator.h"
#include "ForceGenerators/ExplosionForceGenerator.h"
#include <cmath>
#include <vector>

// juego (World of Goo en 3D): bolas de goo unidas por hebras que son muelles
// H viento, E explosión
class GooScene : public PhysicsScene {
public:
	explicit GooScene(std::string name) : PhysicsScene(std::move(name)) {}

	void init() override {
		m_gravity = new GravityForceGenerator(Vector3D(0.f, -9.8f, 0.f));
		m_ground = new GroundForceGenerator(0.f, m_ballRadius, 2000.f, 40.f, 2.f);
		// viento hacia abajo en toda la zona de la pirámide; empieza apagado
		m_wind = new WindForceGenerator(Vector3D(0.f, -200.f, 0.f), Vector3D(0.f, 5.f, 0.f), 20.f, 1.f);
		m_wind->setActive(false);
		// explosión en el suelo a un lado de la pirámide
		m_explosion = new ExplosionForceGenerator(Vector3D(-8.f, 1.f, 0.f), 30.f, 5000.f, 0.3f);
		addForce(m_gravity);
		addForce(m_ground);
		addForce(m_wind);
		addForce(m_explosion);

		// la estructura: sin aceleración fija (la gravedad va como fuerza) y zona de acción grande
		m_structure = new ParticleSystem(Vector3D(), Vector3D(), 200.f);
		m_structure->addForceGenerator(m_gravity);
		m_structure->addForceGenerator(m_ground);
		m_structure->addForceGenerator(m_wind);
		m_structure->addForceGenerator(m_explosion);
		addSystem(m_structure);

		buildPyramid();

		// suelo para verlo
		m_floorTr = physx::PxTransform(physx::PxVec3(0.f, -0.1f, 0.f));
		m_floor = new RenderItem(CreateShape(physx::PxBoxGeometry(50.f, 0.1f, 50.f)), &m_floorTr, Vector4(0.45f, 0.35f, 0.25f, 1.f));
	}

	// si un frame tarda mucho, se divide en pasos pequeños: con pasos grandes los muelles explotan
	void update(double dt) override {
		if (dt > 0.25) dt = 0.25;   // por si la ventana se congela (al arrastrarla, en un breakpoint...)
		while (dt > 0.0) {
			double step = dt;
			if (step > m_maxStep) step = m_maxStep;
			PhysicsScene::update(step);
			dt -= step;
		}

		// las hebras se dibujan donde han quedado las bolas
		for (Strand* s : m_strands) {
			s->update();
		}
	}

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {
		switch (key) {
		case 'h': case 'H':
			toggleForce(m_wind, "Viento");
			break;
		case 'e': case 'E':
			m_explosion->explode();
			break;
		}
	}

	void cleanup() override {
		for (Strand* s : m_strands) delete s;   // solo el dibujo; los muelles los borra PhysicsScene
		m_strands.clear();
		PhysicsScene::cleanup();   // borra el sistema (con las bolas) y las fuerzas (con los muelles)
		m_nodes.clear();
		if (m_floor) {
			m_floor->release();
			m_floor = nullptr;
		}
	}

private:
	const float m_ballRadius = 1.f;
	const float m_strandLength = 10.f;
	const float m_strandK = 50.f;         // dureza de las hebras (blandas, para que el viento las mueva)
	const float m_strandDamping = 8.f;    // amortiguación de las hebras
	const double m_maxStep = 1.0 / 60.0;

	ParticleSystem* m_structure{ nullptr };
	ForceGenerator* m_gravity{ nullptr };
	ForceGenerator* m_ground{ nullptr };
	ForceGenerator* m_wind{ nullptr };
	ExplosionForceGenerator* m_explosion{ nullptr };
	std::vector<Particle*> m_nodes;     // bolas de la estructura (las borra su sistema, aquí solo se apuntan)
	std::vector<Strand*> m_strands;     // dibujo de las hebras

	physx::PxTransform m_floorTr;
	RenderItem* m_floor{ nullptr };

	Particle* addNode(Vector3D pos) {
		ParticleProperties props;
		props.radius = m_ballRadius;
		props.color = Vector4(0.15f, 0.15f, 0.15f, 1.f);   // negras, como las de World of Goo
		props.lifeTime = 1e9;                              // no mueren
		props.damping = 0.9f;
		Particle* p = new Particle(pos, Vector3D(), Vector3D(), props);
		m_structure->addParticle(p);
		m_nodes.push_back(p);
		return p;
	}

	// la hebra es una fuerza más: la escena la borra y el sistema la aplica
	void addStrand(Particle* a, Particle* b) {
		float length = (a->getPos() - b->getPos()).magnitude();
		SpringForceGenerator* spring = new SpringForceGenerator(a, b, m_strandK, length, m_strandDamping);
		addForce(spring);
		m_structure->addForceGenerator(spring);
		m_strands.push_back(new Strand(spring));
	}

	// pega una bola al suelo donde está ahora, con un muelle muy duro
	void anchor(Particle* p) {
		AnchoredSpringForceGenerator* a = new AnchoredSpringForceGenerator(p, p->getPos(), 1000.f, 30.f);
		addForce(a);
		m_structure->addForceGenerator(a);
	}

	// tetraedro: 3 bolas abajo pegadas al suelo y 1 arriba, todas unidas entre sí (6 hebras)
	void buildPyramid() {
		float L = m_strandLength;
		float height = L * std::sqrt(2.f / 3.f);   // altura de un tetraedro regular
		float r = L / std::sqrt(3.f);              // del centro de la base a cada esquina

		Particle* a = addNode(Vector3D(0.f, m_ballRadius, r));
		Particle* b = addNode(Vector3D(-L / 2.f, m_ballRadius, -r / 2.f));
		Particle* c = addNode(Vector3D(L / 2.f, m_ballRadius, -r / 2.f));
		Particle* top = addNode(Vector3D(0.f, m_ballRadius + height, 0.f));

		// como en World of Goo, la base está pegada al suelo (si no, el viento la arrastra)
		anchor(a);
		anchor(b);
		anchor(c);

		addStrand(a, b);
		addStrand(b, c);
		addStrand(c, a);
		addStrand(a, top);
		addStrand(b, top);
		addStrand(c, top);
	}
};