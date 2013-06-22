
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
	x += u.x;
	y += u.y;
}


/*
 *	Subtraction.
 */
Vector2 Vector2::operator - (const Vector2 & u) const {
	return Vector2 (x - u.x, y - u.y);
}

void Vector2::operator -= (const Vector2 & u) {
	x -= u.x;
	y -= u.y;
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
	x *= s;
	y *= s;
}

Vector2 operator * (float s, const Vector2 & v) {

	return Vector2(v.x * s, v.y * s);
}

Vector2 Vector2::operator * (const Vector2 & u) const {
	return Vector2 (x * u.x, y * u.y);
}

void Vector2::operator *= (const Vector2 & u) {
	x *= u.x;
	y *= u.y;
}


/*
 *	Division.
 */
Vector2 Vector2::operator / (const float s) const {
	return Vector2 (x / s, y / s);
}

void Vector2::operator /= (const float s) {
	x /= s;
	y /= s;
}

Vector2 Vector2::operator / (const Vector2 & u) const {
	return Vector2 (x / u.x, y / u.y);
}

void Vector2::operator /= (const Vector2 & u) {
	x /= u.x;
	y /= u.y;
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
