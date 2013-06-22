
#include "Math.h"


const Matrix4 Matrix4::IDENTITY = Matrix4(1.0f);

/*
 *	Constructor
 */
Matrix4::Matrix4 () {}

Matrix4::Matrix4 (float d) {

	m_M[0][0] = d;
	m_M[1][1] = d;
	m_M[2][2] = d;
	m_M[3][3] = d;
}

Matrix4::Matrix4 (float a, float b, float c, float d) {

	m_M[0][0] = a;
	m_M[1][1] = b;
	m_M[2][2] = c;
	m_M[3][3] = d;
}

Matrix4::Matrix4 (const Vector4 & d) {

	m_M[0][0] = d.x;
	m_M[1][1] = d.y;
	m_M[2][2] = d.z;
	m_M[3][3] = d.w;
}

/*
 *	Accessors.
 */
Vector4 & Matrix4::operator [] (const int i) {
	assert(i >= 0 && i < 4);
	return m_M[i];
}

Vector4 Matrix4::operator [] (const int i) const {
	assert(i >= 0 && i < 4);
	return m_M[i];
}


/*
 *	Addition.
 */
Matrix4 Matrix4::operator + (const Matrix4 & N) const {
	Matrix4 R;
	for (int i = 0; i < 4; i++) {
		R[i] = m_M[i] + N[i];
	}
	return R;
}

void Matrix4::operator += (const Matrix4 & N) {
	for (int i = 0; i < 4; i++) {
		m_M[i] += N[i];
	}
}


/*
 *	Subtraction.
 */
Matrix4 Matrix4::operator - (const Matrix4 & N) const {
	Matrix4 R;
	for (int i = 0; i < 4; i++) {
		R[i] = m_M[i] - N[i];
	}
	return R;
}

void Matrix4::operator -= (const Matrix4 & N) {
	for (int i = 0; i < 4; i++) {
		m_M[i] -= N[i];
	}
}

Matrix4 Matrix4::operator - () const {
	Matrix4 R;
	for (int i = 0; i < 4; i++) {
		R[i] = - m_M[i];
	}
	return R;
}


/*
 *	Multiplication.
 */
Matrix4 Matrix4::operator * (const float s) const {
	Matrix4 R;
	for (int i = 0; i < 4; i++) {
		R[i] = m_M[i] * s;
	}
	return R;
}

void Matrix4::operator *= (const float s) {
	for (int i = 0; i < 4; i++) {
		m_M[i] *= s;
	}
}

Vector4 Matrix4::operator * (const Vector4 & v) const {
	Vector4 u;
	for (int i = 0; i < 4; i++) {
		u[i] = m_M[i].DotProduct(v);
	}
	return u;
}


Matrix4 Matrix4::operator * (const Matrix4 & N) const {
	Matrix4 R;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				R[i][j] += m_M[i][k] * N[k][j];
			}
		}
	}
	return R;
}

void Matrix4::operator *= (const Matrix4 & N) {
	Matrix4 R;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				R[i][j] += m_M[i][k] * N[k][j];
			}
		}
	}
	* this = R;
}


/*
 *	Division.
 */
Matrix4 Matrix4::operator / (const float s) const {
	Matrix4 R;
	for (int i = 0; i < 4; i++) {
		R[i] = m_M[i] / s;
	}
	return R;
}

void Matrix4::operator /= (const float s) {
	for (int i = 0; i < 4; i++) {
		m_M[i] /= s;
	}
}


/*
 *	Misc.
 */
Matrix4 Matrix4::Transpose() const {
	Matrix4 R;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			R[i][j] = m_M[j][i];
		}
	}
	return R;
}

Matrix4 Matrix4::Inverse () const {

	Matrix4 C;
	float det;

	C[0][0] = m_M[1][1] * m_M[2][2] * m_M[3][3] - 
		m_M[1][1] * m_M[2][3] * m_M[3][2] - 
		m_M[2][1] * m_M[1][2] * m_M[3][3] + 
		m_M[2][1] * m_M[1][3] * m_M[3][2] +
		m_M[3][1] * m_M[1][2] * m_M[2][3] - 
		m_M[3][1] * m_M[1][3] * m_M[2][2];

	C[1][0] = -m_M[1][0] * m_M[2][2] * m_M[3][3] + 
		m_M[1][0] * m_M[2][3] * m_M[3][2] + 
		m_M[2][0] * m_M[1][2] * m_M[3][3] - 
		m_M[2][0] * m_M[1][3] * m_M[3][2] - 
		m_M[3][0] * m_M[1][2] * m_M[2][3] + 
		m_M[3][0] * m_M[1][3] * m_M[2][2];

	C[2][0] = m_M[1][0] * m_M[2][1] * m_M[3][3] -
		m_M[1][0] * m_M[2][3] * m_M[3][1] -
		m_M[2][0] * m_M[1][1] * m_M[3][3] +
		m_M[2][0] * m_M[1][3] * m_M[3][1] +
		m_M[3][0] * m_M[1][1] * m_M[2][3] -
		m_M[3][0] * m_M[1][3] * m_M[2][1];

	C[3][0] = -m_M[1][0] * m_M[2][1] * m_M[3][2] +
		m_M[1][0] * m_M[2][2] * m_M[3][1] +
		m_M[2][0] * m_M[1][1] * m_M[3][2] -
		m_M[2][0] * m_M[1][2] * m_M[3][1] -
		m_M[3][0] * m_M[1][1] * m_M[2][2] +
		m_M[3][0] * m_M[1][2] * m_M[2][1];

	// det must not be 0.
	det = m_M[0][0] * C[0][0] + 
		m_M[0][1] * C[1][0] + 
		m_M[0][2] * C[2][0] + 
		m_M[0][3] * C[3][0];


	C[0][1] = -m_M[0][1] * m_M[2][2] * m_M[3][3] +
		m_M[0][1] * m_M[2][3] * m_M[3][2] +
		m_M[2][1] * m_M[0][2] * m_M[3][3] -
		m_M[2][1] * m_M[0][3] * m_M[3][2] -
		m_M[3][1] * m_M[0][2] * m_M[2][3] +
		m_M[3][1] * m_M[0][3] * m_M[2][2];

	C[1][1] = m_M[0][0] * m_M[2][2] * m_M[3][3] -
		m_M[0][0] * m_M[2][3] * m_M[3][2] -
		m_M[2][0] * m_M[0][2] * m_M[3][3] +
		m_M[2][0] * m_M[0][3] * m_M[3][2] +
		m_M[3][0] * m_M[0][2] * m_M[2][3] -
		m_M[3][0] * m_M[0][3] * m_M[2][2];

	C[2][1] = -m_M[0][0] * m_M[2][1] * m_M[3][3] +
		m_M[0][0] * m_M[2][3] * m_M[3][1] +
		m_M[2][0] * m_M[0][1] * m_M[3][3] -
		m_M[2][0] * m_M[0][3] * m_M[3][1] -
		m_M[3][0] * m_M[0][1] * m_M[2][3] +
		m_M[3][0] * m_M[0][3] * m_M[2][1];

	C[3][1] = m_M[0][0] * m_M[2][1] * m_M[3][2] -
		m_M[0][0] * m_M[2][2] * m_M[3][1] -
		m_M[2][0] * m_M[0][1] * m_M[3][2] +
		m_M[2][0] * m_M[0][2] * m_M[3][1] +
		m_M[3][0] * m_M[0][1] * m_M[2][2] -
		m_M[3][0] * m_M[0][2] * m_M[2][1];

	C[0][2] = m_M[0][1] * m_M[1][2] * m_M[3][3] -
		m_M[0][1] * m_M[1][3] * m_M[3][2] -
		m_M[1][1] * m_M[0][2] * m_M[3][3] +
		m_M[1][1] * m_M[0][3] * m_M[3][2] +
		m_M[3][1] * m_M[0][2] * m_M[1][3] -
		m_M[3][1] * m_M[0][3] * m_M[1][2];

	C[1][2] = -m_M[0][0] * m_M[1][2] * m_M[3][3] +
		m_M[0][0] * m_M[1][3] * m_M[3][2] +
		m_M[1][0] * m_M[0][2] * m_M[3][3] -
		m_M[1][0] * m_M[0][3] * m_M[3][2] -
		m_M[3][0] * m_M[0][2] * m_M[1][3] +
		m_M[3][0] * m_M[0][3] * m_M[1][2];

	C[2][2] = m_M[0][0] * m_M[1][1] * m_M[3][3] -
		m_M[0][0] * m_M[1][3] * m_M[3][1] -
		m_M[1][0] * m_M[0][1] * m_M[3][3] +
		m_M[1][0] * m_M[0][3] * m_M[3][1] +
		m_M[3][0] * m_M[0][1] * m_M[1][3] -
		m_M[3][0] * m_M[0][3] * m_M[1][1];

	C[3][2] = -m_M[0][0] * m_M[1][1] * m_M[3][2] +
		m_M[0][0] * m_M[1][2] * m_M[3][1] +
		m_M[1][0] * m_M[0][1] * m_M[3][2] -
		m_M[1][0] * m_M[0][2] * m_M[3][1] -
		m_M[3][0] * m_M[0][1] * m_M[1][2] +
		m_M[3][0] * m_M[0][2] * m_M[1][1];

	C[0][3] = -m_M[0][1] * m_M[1][2] * m_M[2][3] +
		m_M[0][1] * m_M[1][3] * m_M[2][2] +
		m_M[1][1] * m_M[0][2] * m_M[2][3] -
		m_M[1][1] * m_M[0][3] * m_M[2][2] -
		m_M[2][1] * m_M[0][2] * m_M[1][3] +
		m_M[2][1] * m_M[0][3] * m_M[1][2];

	C[1][3] = m_M[0][0] * m_M[1][2] * m_M[2][3] -
		m_M[0][0] * m_M[1][3] * m_M[2][2] -
		m_M[1][0] * m_M[0][2] * m_M[2][3] +
		m_M[1][0] * m_M[0][3] * m_M[2][2] +
		m_M[2][0] * m_M[0][2] * m_M[1][3] -
		m_M[2][0] * m_M[0][3] * m_M[1][2];

	C[2][3] = -m_M[0][0] * m_M[1][1] * m_M[2][3] +
		m_M[0][0] * m_M[1][3] * m_M[2][1] +
		m_M[1][0] * m_M[0][1] * m_M[2][3] -
		m_M[1][0] * m_M[0][3] * m_M[2][1] -
		m_M[2][0] * m_M[0][1] * m_M[1][3] +
		m_M[2][0] * m_M[0][3] * m_M[1][1];

	C[3][3] = m_M[0][0] * m_M[1][1] * m_M[2][2] -
		m_M[0][0] * m_M[1][2] * m_M[2][1] -
		m_M[1][0] * m_M[0][1] * m_M[2][2] +
		m_M[1][0] * m_M[0][2] * m_M[2][1] +
		m_M[2][0] * m_M[0][1] * m_M[1][2] -
		m_M[2][0] * m_M[0][2] * m_M[1][1];

	return C / det;
}


/*
 *	Conversion.
 */
Matrix4::operator float * () {
	return * m_M;
}


Matrix4	Matrix4::Rotation (float x, float y, float z) {

	float cos_x = cos(x);
	float sin_x = sin(x);
	float cos_y = cos(y);
	float sin_y = sin(y);
	float cos_z = cos(z);
	float sin_z = sin(z);

	float sx_sy = sin_x * sin_y;
	float cx_sy = cos_x * sin_y;

	Matrix4 Rotation = Matrix4::IDENTITY;
	Rotation[0][0] =  cos_y * cos_z;
	Rotation[0][1] = -cos_y * sin_z;
	Rotation[0][2] =  sin_y;
	Rotation[1][0] =  cos_x * sin_z + sx_sy * cos_z;
	Rotation[1][1] =  cos_x * cos_z - sx_sy * sin_z;
	Rotation[1][2] = -sin_x * cos_y;
	Rotation[2][0] =  sin_x * sin_z - cx_sy * cos_z;
	Rotation[2][1] =  sin_x * cos_z + cx_sy * sin_z;
	Rotation[2][2] =  cos_x * cos_y;

	return Rotation;
}

Matrix4	Matrix4::Rotation (const Vector3 & r) {

	float cos_x = cos(r.x);
	float sin_x = sin(r.x);
	float cos_y = cos(r.y);
	float sin_y = sin(r.y);
	float cos_z = cos(r.z);
	float sin_z = sin(r.z);

	float sx_sy = sin_x * sin_y;
	float cx_sy = cos_x * sin_y;

	Matrix4 Rotation = Matrix4::IDENTITY;
	Rotation[0][0] =  cos_y * cos_z;
	Rotation[0][1] = -cos_y * sin_z;
	Rotation[0][2] =  sin_y;
	Rotation[1][0] =  cos_x * sin_z + sx_sy * cos_z;
	Rotation[1][1] =  cos_x * cos_z - sx_sy * sin_z;
	Rotation[1][2] = -sin_x * cos_y;
	Rotation[2][0] =  sin_x * sin_z - cx_sy * cos_z;
	Rotation[2][1] =  sin_x * cos_z + cx_sy * sin_z;
	Rotation[2][2] =  cos_x * cos_y;

	return Rotation;
}

Matrix4	Matrix4::RotationX (float x) {

	float cos_x = cos(x);
	float sin_x = sin(x);

	Matrix4 RotationX = Matrix4::IDENTITY;
	RotationX[1][1] =  cos_x;
	RotationX[1][2] = -sin_x;
	RotationX[2][1] =  sin_x;
	RotationX[2][2] =  cos_x;

	return RotationX;
}

Matrix4	Matrix4::RotationY (float y) {

	float cos_y = cos(y);
	float sin_y = sin(y);

	Matrix4 RotationY = Matrix4::IDENTITY;
	RotationY[0][0] =  cos_y;
	RotationY[0][2] =  sin_y;
	RotationY[2][0] = -sin_y;
	RotationY[2][2] =  cos_y;

	return RotationY;
}

Matrix4	Matrix4::RotationZ (float z) {

	float cos_z = cos(z);
	float sin_z = sin(z);

	Matrix4 RotationZ = Matrix4::IDENTITY;
	RotationZ[0][0] =  cos_z;
	RotationZ[0][1] = -sin_z;
	RotationZ[1][0] =  sin_z;
	RotationZ[1][1] =  cos_z;

	return RotationZ;
}

Matrix4 Matrix4::Rotation (const Vector3 & axis, float a) {

	// Creates a rotation matrix about the specified axis.
	// The axis must be a unit vector. The angle must be in degrees.
	//
	// Let u = axis of rotation = (x, y, z)
	//
	//        | xx(1 - c) + c   xy(1 - c) + zs  xz(1 - c) - ys   0 |
	// R(a) = | yx(1 - c) - zs  yy(1 - c) + c   yz(1 - c) + xs   0 |
	//        | zx(1 - c) - ys  zy(1 - c) - xs  zz(1 - c) + c    0 |
	//        |      0              0                0           1 |
	//
	// Where:
	//	c = cos(a)
	//  s = sin(a)

	Matrix4 Rotation = Matrix4::IDENTITY;

	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	float cos_a = cos(a);
	float sin_a = sin(a);
	float one_minus_cos_a = 1.0f - cos_a;

	float xy = x * y;
	float yz = y * z;
	float xz = x * z;

	float xsin_a = x * sin_a;
	float ysin_a = y * sin_a;
	float zsin_a = z * sin_a;

	Rotation[0][0] = x * x * one_minus_cos_a +  cos_a;
	Rotation[0][1] = xy	   * one_minus_cos_a + zsin_a;
	Rotation[0][2] = xz	   * one_minus_cos_a - ysin_a;

	Rotation[1][0] = xy	   * one_minus_cos_a - zsin_a;
	Rotation[1][1] = y * y * one_minus_cos_a +  cos_a;
	Rotation[1][2] = yz	   * one_minus_cos_a + xsin_a;

	Rotation[2][0] = xz	   * one_minus_cos_a + ysin_a;
	Rotation[2][1] = yz    * one_minus_cos_a - xsin_a;
	Rotation[2][2] = z * z * one_minus_cos_a +  cos_a;

	return Rotation;
}