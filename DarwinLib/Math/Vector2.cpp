
#include "Math.h"


/* 
 *	Constructors.
 */
Vector2::Vector2 () : 
	x(0.0f), y(0.0f) {}

Vector2::Vector2 (float xy) :
	x(xy), y(xy) {}

Vector2::Vector2 (float _x, float _y) :
	x(_x), y(_y) {}


/*
 *	Accessors.
 */
float & Vector2::operator [] (const int i) {
	assert(i >= 0 && i < 2);
	return (&x)[i];
}

float Vector2::operator [] (const int i) const {
	assert(i >= 0 && i < 2);
	return (&x)[i];
}


/*
 *	Addition.
 */
Vector2 Vector2::operator + (const Vector2 & u) const {
	return Vector2 (x + u.x, y + u.y);
}

void Vector2::operator += (const Vector2 & u) {
	for (int i = 0; i < 2; i++) {
		(* this)[i] += u[i];
	}
}


/*
 *	Subtraction.
 */
Vector2 Vector2::operator - (const Vector2 & u) const {
	return Vector2 (x - u.x, y - u.y);
}

void Vector2::operator -= (const Vector2 & u) {
	for (int i = 0; i < 2; i++) {
		(* this)[i] -= u[i];
	}
}

Vector2 Vector2::operator - () const {
	return Vector2 (-x, -y);
}


/*
 *	Multiplication.
 */
Vector2 Vector2::operator * (const float s) const {
	return Vector2 (x * s, y * s);
}

void Vector2::operator *= (const float s) {
	for (int i = 0; i < 2; i++) {
		(* this)[i] *= s;
	}
}

Vector2 Vector2::operator * (const Vector2 & u) const {
	return Vector2 (x * u.x, y * u.y);
}

void Vector2::operator *= (const Vector2 & u) {
	for (int i = 0; i < 2; i++) {
		(* this)[i] *= u[i];
	}
}


/*
 *	Division.
 */
Vector2 Vector2::operator / (const float s) const {
	return Vector2 (x / s, y / s);
}

void Vector2::operator /= (const float s) {
	for (int i = 0; i < 2; i++) {
		(* this)[i] /= s;
	}
}

Vector2 Vector2::operator / (const Vector2 & u) const {
	return Vector2 (x / u.x, y / u.y);
}

void Vector2::operator /= (const Vector2 & u) {
	for (int i = 0; i < 2; i++) {
		(* this)[i] /= u[i];
	}
}


/*
 *	Misc.
 */
float Vector2::DotProduct (const Vector2 & u) const {
	return x * u.x + y * u.y;
}


float Vector2::Length () const {
	return sqrtf(x * x + y * y);
}

Vector2 Vector2::Normalize () const {
	return (* this) / Length();
}


/*
 *	Conversions.
 */
Vector2::operator Vector3 () {
	return Vector3 (x, y, 0.0f);
}

Vector2::operator Vector4 () {
	return Vector4 (x, y, 0.0f, 0.0f);
}

Vector2::operator float * () {
	return &x;
}
