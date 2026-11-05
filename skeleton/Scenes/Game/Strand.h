#pragma once
#include "ForceGenerators/SpringForceGenerator.h"
#include "RenderUtils.hpp"
#include <cmath>

// dibujo de una hebra: una fila de bolitas entre sus dos bolas
// el color dice cómo trabaja el muelle: rojo si está estirado, azul si está comprimido
class Strand
{
public:
	explicit Strand(SpringForceGenerator* spring) : m_spring(spring) {
		for (int i = 0; i < BEADS; i++) {
			m_beadTr[i] = physx::PxTransform(physx::PxVec3(0.f, 0.f, 0.f));
			m_beads[i] = new RenderItem(CreateShape(physx::PxSphereGeometry(0.3f)), &m_beadTr[i], Vector4(1.f, 1.f, 1.f, 1.f));
		}
		update();
	}

	~Strand() {
		for (int i = 0; i < BEADS; i++) {
			m_beads[i]->release();
		}
	}

	// el render item apunta a m_beadTr, así que no se puede copiar
	Strand(const Strand&) = delete;
	Strand& operator=(const Strand&) = delete;

	// recoloca las bolitas entre las dos bolas y les pone el color según lo estirada que esté
	void update() {
		Vector3D a = m_spring->getA()->getPos();
		Vector3D b = m_spring->getB()->getPos();

		for (int i = 0; i < BEADS; i++) {
			float t = float(i + 1) / float(BEADS + 1);   // repartidas entre las dos bolas, sin tocarlas
			m_beadTr[i].p = a + (b - a) * t;
		}

		// cuánto ha cambiado respecto a su longitud natural (0.1 = un 10 % más larga)
		float restLength = m_spring->getRestLength();
		float stretch = ((b - a).magnitude() - restLength) / restLength;

		// con un 10 % ya sale el color del todo
		float amount = std::fabs(stretch) * 10.f;
		if (amount > 1.f) amount = 1.f;

		Vector4 target = Vector4(1.f, 0.2f, 0.2f, 1.f);              // estirada: roja
		if (stretch < 0.f) target = Vector4(0.2f, 0.4f, 1.f, 1.f);   // comprimida: azul

		// mezcla entre el color neutro y el de la tensión
		Vector4 color(m_neutral.x + (target.x - m_neutral.x) * amount,
			m_neutral.y + (target.y - m_neutral.y) * amount,
			m_neutral.z + (target.z - m_neutral.z) * amount, 1.f);
		for (int i = 0; i < BEADS; i++) {
			m_beads[i]->color = color;
		}
	}

private:
	static const int BEADS = 15;   // bolitas por hebra

	SpringForceGenerator* m_spring;
	physx::PxTransform m_beadTr[BEADS];
	RenderItem* m_beads[BEADS];
	Vector4 m_neutral = Vector4(0.9f, 0.9f, 0.85f, 1.f);   // color en reposo
};