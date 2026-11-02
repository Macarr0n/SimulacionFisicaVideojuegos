#pragma once
#include <PxPhysics.h>

using namespace std;

class Vector3D
{
private:
	float x, y, z;

public:
	Vector3D() : x(0), y(0), z(0) {};
	Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
	Vector3D(physx::PxVec3 vec) : x(vec.x), y(vec.y), z(vec.z) {};

	float magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}

	Vector3D normalize() const {
		float modulo = magnitude();
		return Vector3D(x / modulo, y / modulo, z / modulo);
	}

	float dot(const Vector3D& v) const {
		return (x * v.x + y * v.y + z * v.z);
	}

	Vector3D cross(const Vector3D& v) const {
		return Vector3D((y * v.z - z * v.y), (z * v.x - x * v.z), (x * v.y - y * v.x));
	}

	Vector3D& operator= (const Vector3D& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	Vector3D& operator+ (const Vector3D& other) const {
		return Vector3D(x + other.x, y + other.y, z + other.z);
	}

	Vector3D& operator- (const Vector3D& other) const {
		return Vector3D(x - other.x, y - other.y, z - other.z);
	}

	Vector3D& operator* (float op) const {
		return Vector3D(x * op, y * op, z * op);
	}

	Vector3D& operator+= (const Vector3D& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return Vector3D(x, y, z);
	}

	operator physx::PxVec3() const {
		return physx::PxVec3(x, y, z);
	}
};

