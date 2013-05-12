
#include "Math.h"


/*
 *	Constructor
 */
Matrix4::Matrix4 () {
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


/*
 *	Static generators.
 */
Matrix4 Matrix4::Identity () {

	Matrix4 M;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			M[i][j] = (i == j);
		}
	}

	return M;
}

Matrix4 Matrix4::Zeroes () {

	Matrix4 M;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			M[i][j] = 0.0;
		}
	}

	return M;
}

Matrix4 Matrix4::Diag (float d) {
	Matrix4 M;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				M[i][j] = d;
			} else {
				M[i][j] = 0.0;
			}
		}
	}
	return M;
}
