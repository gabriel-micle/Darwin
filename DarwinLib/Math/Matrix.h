
#pragma once


/*
 *	Matrix4 Class
 *
 *	Represented in row-major format as:
 *	+-----+-----+-----+-----+
 *	| m00 | m01 | m02 | m03 |
 *	+-----+-----+-----+-----+
 *	| m10 | m11 | m12 | m13 |
 *	+-----+-----+-----+-----+
 *	| m20 | m21 | m22 | m23 |
 *	+-----+-----+-----+-----+
 *	| m30 | m31 | m32 | m33 |
 *	+-----+-----+-----+-----+
 */
class Matrix4 {

private:
	Vector4			m_M[4];

public:
	
	static const Matrix4 IDENTITY;

					Matrix4 ();
					Matrix4 (float);
					Matrix4 (float, float, float, float);
					Matrix4 (const Vector4 &);

	Vector4 &		operator [] (const int i);
	Vector4			operator [] (const int i) const;

	bool			operator == (const Matrix4 & N) const;

	Matrix4			operator + (const Matrix4 & N) const;
	void			operator += (const Matrix4 & N);

	Matrix4			operator - (const Matrix4 & N) const;
	void			operator -= (const Matrix4 & N);

	Matrix4			operator - () const;

	Matrix4			operator * (const float s) const;
	void			operator *= (const float s);

	Matrix4			operator / (const float s) const;
	void			operator /= (const float s);

	Vector4			operator * (const Vector4 & v) const;

	Matrix4			operator * (const Matrix4 & N) const;
	void			operator *= (const Matrix4 & N);

	Matrix4			Transpose () const;
	Matrix4			Inverse () const;

					operator float * ();

	static Matrix4	Rotation (const Vector3 & axis, float a);
	static Matrix4	Rotation (float x, float y, float z);
	static Matrix4	Rotation (const Vector3 & r);
	static Matrix4	RotationX (float x);
	static Matrix4	RotationY (float y);
	static Matrix4	RotationZ (float z);

};
