#pragma once

struct TipoProyectil {
	float masaReal;
	float velReal;
	float velSim;
	float gravedadReal = 9.8f;

	float masaSim() const {
		float r = velReal / velSim;
		return masaReal * r * r;
	}

	float gravedadSim() const {
		float r = velSim / velReal;
		return gravedadReal * r * r;
	}
};