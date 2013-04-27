
#include "Math.h"


/* 
 *	Constructors.
 */
Vector3::Vector3 () : 
	x(0.0f), y(0.0f), z(0.0f) {}

Vector3::Vector3 (float xyz) :
	x(xyz), y(xyz), z(xyz) {}

Vector3::Vector3 (float _x, float _y, float _z) :
	x(_x), y(_y), z(_z) {}


/*
 *	Accessors.
 */
float & Vector3::operator [] (const int i) {
	assert(i >= 0 && i < 3);
	return (&x)[i];
}

float Vector3::operator [] (const int i) const {
	assert(i >= 0 && i < 3);
	return (&x)[i];
}


/*
 *	Addition.
 */
Vector3 Vector3::operator + (const Vector3 & u) const {
	return Vector3 (x + u.x, y + u.y, z + u.z);
}

void Vector3::operator += (const Vector3 & u) {
	for (int i = 0; i < 3; i++) {
		(* this)[i] += u[i];
	}
}


/*
 *	Subtraction.
 */
Vector3 Vector3::operator - (const Vector3 & u) const {
	return Vector3 (x - u.x, y - u.y, z - u.z);
}

void Vector3::operator -= (const Vector3 & u) {
	for (int i = 0; i < 3; i++) {
		(* this)[i] -= u[i];
	}
}

Vector3 Vector3::operator - () const {
	return Vector3 (-x, -y, -z);
}


/*
 *	Multiplication.
 */
Vector3 Vector3::operator * (const float s) const {
	return Vector3 (x * s, y * s, z * s);
}

void Vector3::operator *= (const float s) {
	for (int i = 0; i < 3; i++) {
		(* this)[i] *= s;
	}
}

Vector3 Vector3::operator * (const Vector3 & u) const {
	return Vector3 (x * u.x, y * u.y, z * u.z);
}

void Vector3::operator *= (const Vector3 & u) {
	for (int i = 0; i < 3; i++) {
		(* this)[i] *= u[i];
	}
}


/*
 *	Division.
 */
Vector3 Vector3::operator / (const float s) const {
	return Vector3 (x / s, y / s, z / s);
}

void Vector3::operator /= (const float s) {
	for (int i = 0; i < 3; i++) {
		(* this)[i] /= s;
	}
}

Vector3 Vector3::operator / (const Vector3 & u) const {
	return Vector3 (x / u.x, y / u.y, z / u.z);
}

void Vector3::operator /= (const Vector3 & u) {
	for (int i = 0; i < 3; i++) {
		(* this)[i] /= u[i];
	}
}


/*
 *	Misc.
 */
float Vector3::dotProduct (const Vector3 & u) const {
	return x * u.x + y * u.y + z * u.z;
}

Vector3 Vector3::crossProduct (const Vector3 & u) const {
	return Vector3 (
		y * u.z - z * u.y,
		z * u.x - x * u.z,
		x * u.y - y * u.x);
}

float Vector3::length () const {
	return sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::normalize () const {
	return (* this) / length();
}


/*
 *	Conversions.
 */
Vector3::operator Vector2 () {
	return Vector2 (x, y);
}

Vector3::operator Vector4 () {
	return Vector4 (x, y, z, 0.0f);
}

Vector3::operator float * () {
	return &x;
}
