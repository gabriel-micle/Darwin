
#ifndef _MATRIX_H_
#define _MATRIX_H_


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
	Vector4		M[4];

public:
	
				Matrix4 ();

	Vector4 &	operator [] (const int i);
	Vector4		operator [] (const int i) const;

	bool		operator == (const Matrix4 & N) const;

	Matrix4		operator + (const Matrix4 & N) const;
	void		operator += (const Matrix4 & N);

	Matrix4		operator - (const Matrix4 & N) const;
	void		operator -= (const Matrix4 & N);

	Matrix4		operator - () const;

	Matrix4		operator * (const float s) const;
	void		operator *= (const float s);

	Matrix4		operator / (const float s) const;
	void		operator /= (const float s);

	Vector4		operator * (const Vector4 & v) const;

	Matrix4		operator * (const Matrix4 & N) const;
	void		operator *= (const Matrix4 & N);

	Matrix4		transpose() const;
	Matrix4		inverse () const;

				operator float * ();

	static Matrix4	identity ();
	static Matrix4	zeroes ();
	static Matrix4	diag (float d);
};

#endif
