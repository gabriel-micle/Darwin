
#pragma once


/*
 * Predeclare incomplete classes.
 */
class Vector2;
class Vector3;
class Vector4;
class Matrix4;


/*
 *	Vector2 Class
 *
 *	Represented as a column:
 *	+----+
 *	| v0 |
 *	+----+
 *	| v1 |
 *	+----+
 */
class Vector2 {

	friend Vector2 operator * (float s, const Vector2 & v);

public:

	float		x, y;

				Vector2 ();
				Vector2 (float xy);
				Vector2 (float x, float y);

	float &		operator [] (const int i);
	float		operator [] (const int i) const;

	Vector2		operator + (const Vector2 & u) const;
	void		operator += (const Vector2 & u);

	Vector2		operator - (const Vector2 & u) const;
	void		operator -= (const Vector2 & u);

	Vector2		operator - () const;

	Vector2		operator * (const float s) const;
	void		operator *= (const float s);

	Vector2		operator * (const Vector2 & u) const;
	void		operator *= (const Vector2 & u);

	Vector2		operator / (const float s) const;
	void		operator /= (const float s);

	Vector2		operator / (const Vector2 & u) const;
	void		operator /= (const Vector2 & u);

	float		Length () const;
	float		DotProduct (const Vector2 & u) const;

	Vector2		Normalize () const;

				operator Vector3 ();
				operator Vector4 ();
				operator float * ();
};


/*
 *	Vector3 Class
 *
 *	Represented as a column:
 *	+----+
 *	| v0 |
 *	+----+
 *	| v1 |
 *	+----+
 *	| v2 |
 *	+----+
 */
class Vector3 {

	friend Vector3 operator * (float s, const Vector3 & v);

public:

	float		x, y, z;

				Vector3 ();
				Vector3 (float xyz);
				Vector3 (float x, float y, float z);

	float &		operator [] (const int i);
	float		operator [] (const int i) const;

	Vector3		operator + (const Vector3 & u) const;
	void		operator += (const Vector3 & u);

	Vector3		operator - (const Vector3 & u) const;
	void		operator -= (const Vector3 & u);

	Vector3		operator - () const;

	Vector3		operator * (const float s) const;
	void		operator *= (const float s);

	Vector3		operator * (const Vector3 & u) const;
	void		operator *= (const Vector3 & u);

	Vector3		operator / (const float s) const;
	void		operator /= (const float s);

	Vector3		operator / (const Vector3 & u) const;
	void		operator /= (const Vector3 & u);

	float		Length () const;
	float		DotProduct (const Vector3 & u) const;
	Vector3		CrossProduct (const Vector3 & u) const;

	Vector3		Normalize () const;

				operator Vector2 ();
				operator Vector4 ();
				operator float * ();
};


/*
 *	Vector4 Class
 *
 *	Represented as a column:
 *	+----+
 *	| v0 |
 *	+----+
 *	| v1 |
 *	+----+
 *	| v2 |
 *	+----+
 *	| v3 |
 *	+----+
 *
 *	When premultiplying with a 4x4 matrix, the transpose is used:
 *	+----+----+----+----+
 *	| v0 | v1 | v2 | v3 |
 *	+----+----+----+----+
 */
class Vector4 {

	friend Vector4 operator * (float s, const Vector4 & v);

public:

	float		x, y, z, w;

				Vector4 ();
				Vector4 (float xyzw);
				Vector4 (float x, float y, float z, float w);
				Vector4 (Vector3 xyz, float w);

	float &		operator [] (const int i);
	float		operator [] (const int i) const;

	Vector4		operator + (const Vector4 & u) const;
	void		operator += (const Vector4 & u);

	Vector4		operator - (const Vector4 & u) const;
	void		operator -= (const Vector4 & u);

	Vector4		operator - () const;

	Vector4		operator * (const float s) const;
	void		operator *= (const float s);

	Vector4		operator * (const Vector4 & u) const;
	void		operator *= (const Vector4 & u);

	Vector4		operator * (const Matrix4 & M) const;
	void		operator *= (const Matrix4 & M);

	Vector4		operator / (const float s) const;
	void		operator /= (const float s);

	Vector4		operator / (const Vector4 & u) const;
	void		operator /= (const Vector4 & u);

	float		Length () const;
	float		DotProduct (const Vector4 & u) const;

	Vector4		Normalize () const;

				operator Vector2 ();
				operator Vector3 ();
				operator float * ();
};
