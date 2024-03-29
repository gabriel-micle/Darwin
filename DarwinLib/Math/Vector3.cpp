
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
	x += u.x;
	y += u.y;
	z += u.z;
}


/*
 *	Subtraction.
 */
Vector3 Vector3::operator - (const Vector3 & u) const {
	return Vector3 (x - u.x, y - u.y, z - u.z);
}

void Vector3::operator -= (const Vector3 & u) {
	x -= u.x;
	y -= u.y;
	z -= u.z;
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
	x *= s;
	y *= s;
	z *= s;
}

Vector3 operator * (float s, const Vector3 & v) {
	return Vector3(v.x * s, v.y * s, v.z * s);
}

Vector3 Vector3::operator * (const Vector3 & u) const {
	return Vector3 (x * u.x, y * u.y, z * u.z);
}

void Vector3::operator *= (const Vector3 & u) {
	x *= u.x;
	y *= u.y;
	z *= u.z;
}


/*
 *	Division.
 */
Vector3 Vector3::operator / (const float s) const {
	return Vector3 (x / s, y / s, z / s);
}

void Vector3::operator /= (const float s) {
	x /= s;
	y /= s;
	z /= s;
}

Vector3 Vector3::operator / (const Vector3 & u) const {
	return Vector3 (x / u.x, y / u.y, z / u.z);
}

void Vector3::operator /= (const Vector3 & u) {
	x /= u.x;
	y /= u.y;
	z /= u.z;
}


/*
 *	Misc.
 */
float Vector3::DotProduct (const Vector3 & u) const {
	return x * u.x + y * u.y + z * u.z;
}

Vector3 Vector3::CrossProduct (const Vector3 & u) const {
	return Vector3 (
		y * u.z - z * u.y,
		z * u.x - x * u.z,
		x * u.y - y * u.x);
}

float Vector3::Length () const {
	return sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::Normalize () const {
	return (* this) / Length();
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
