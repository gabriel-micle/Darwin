
#include "Math.h"


/* 
 *	Constructors.
 */
Vector4::Vector4 () : 
	x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

Vector4::Vector4 (float xyzw) :
	x(xyzw), y(xyzw), z(xyzw), w(xyzw) {}

Vector4::Vector4 (float _x, float _y, float _z, float _w) :
	x(_x), y(_y), z(_z), w(_w) {}

Vector4::Vector4 (Vector3 _xyz, float _w) :
	x(_xyz.x), y(_xyz.y), z(_xyz.z), w(_w) {}


/*
 *	Accessors.
 */
float & Vector4::operator [] (const int i) {
	assert(i >= 0 && i < 4);
	return (&x)[i];
}

float Vector4::operator [] (const int i) const {
	assert(i >= 0 && i < 4);
	return (&x)[i];
}


/*
 *	Addition.
 */
Vector4 Vector4::operator + (const Vector4 & u) const {
	return Vector4 (x + u.x, y + u.y, z + u.z, w + u.w);
}

void Vector4::operator += (const Vector4 & u) {
	for (int i = 0; i < 4; i++) {
		(* this)[i] += u[i];
	}
}


/*
 *	Subtraction.
 */
Vector4 Vector4::operator - (const Vector4 & u) const {
	return Vector4 (x - u.x, y - u.y, z - u.z, 0.0f);
}

void Vector4::operator -= (const Vector4 & u) {
	for (int i = 0; i < 4; i++) {
		(* this)[i] -= u[i];
	}
}

Vector4 Vector4::operator - () const {
	return Vector4 (-x, -y, -z, -w);
}


/*
 *	Multiplication.
 */
Vector4 Vector4::operator * (const float s) const {
	return Vector4 (x * s, y * s, z * s, w * s);
}

void Vector4::operator *= (const float s) {
	for (int i = 0; i < 4; i++) {
		(* this)[i] *= s;
	}
}

Vector4 Vector4::operator * (const Vector4 & u) const {
	return Vector4 (x * u.x, y * u.y, z * u.z, w * u.w);
}

void Vector4::operator *= (const Vector4 & u) {
	for (int i = 0; i < 4; i++) {
		(* this)[i] *= u[i];
	}
}

Vector4 Vector4::operator * (const Matrix4 & M) const {
	Vector4 u;
	for (int i = 0; i < 4; i++) {
		u[i] = M[i].DotProduct(* this);
	}
	return u;
}

void Vector4::operator *= (const Matrix4 & M) {
	Vector4 u;
	for (int i = 0; i < 4; i++) {
		u[i] = M[i].DotProduct(* this);
	}
	* this = u;
}


/*
 *	Division.
 */
Vector4 Vector4::operator / (const float s) const {
	return Vector4 (x / s, y / s, z / s, w / s);
}

void Vector4::operator /= (const float s) {
	for (int i = 0; i < 4; i++) {
		(* this)[i] /= s;
	}
}

Vector4 Vector4::operator / (const Vector4 & u) const {
	return Vector4 (x / u.x, y / u.y, z / u.z, w / u.w);
}

void Vector4::operator /= (const Vector4 & u) {
	for (int i = 0; i < 4; i++) {
		(* this)[i] /= u[i];
	}
}


/*
 *	Misc.
 */
float Vector4::DotProduct (const Vector4 & u) const {
	return x * u.x + y * u.y + z * u.z + w * u.w;
}

float Vector4::Length () const {
	return sqrtf(x * x + y * y + z * z + w * w);
}

Vector4 Vector4::Normalize () const {
	return (* this) / Length();
}


/*
 *	Conversions.
 */
Vector4::operator Vector2 () {
	return Vector2 (x, y);
}

Vector4::operator Vector3 () {
	return Vector3 (x, y, z);
}

Vector4::operator float * () {
	return &x;
}
