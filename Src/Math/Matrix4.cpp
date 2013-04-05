
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
	return M[i];
}

Vector4 Matrix4::operator [] (const int i) const {
	return M[i];
}


/*
 *	Addition.
 */
Matrix4 Matrix4::operator + (const Matrix4 & N) const {
	Matrix4 R;
	for (int i = 0; i < 4; i++) {
		R[i] = M[i] + N[i];
	}
	return R;
}

void Matrix4::operator += (const Matrix4 & N) {
	for (int i = 0; i < 4; i++) {
		M[i] += N[i];
	}
}


/*
 *	Subtraction.
 */
Matrix4 Matrix4::operator - (const Matrix4 & N) const {
	Matrix4 R;
	for (int i = 0; i < 4; i++) {
		R[i] = M[i] - N[i];
	}
	return R;
}

void Matrix4::operator -= (const Matrix4 & N) {
	for (int i = 0; i < 4; i++) {
		M[i] -= N[i];
	}
}

Matrix4 Matrix4::operator - () const {
	Matrix4 R;
	for (int i = 0; i < 4; i++) {
		R[i] = - M[i];
	}
	return R;
}


/*
 *	Multiplication.
 */
Matrix4 Matrix4::operator * (const float s) const {
	Matrix4 R;
	for (int i = 0; i < 4; i++) {
		R[i] = M[i] * s;
	}
	return R;
}

void Matrix4::operator *= (const float s) {
	for (int i = 0; i < 4; i++) {
		M[i] *= s;
	}
}

Vector4 Matrix4::operator * (const Vector4 & v) const {
	Vector4 u;
	for (int i = 0; i < 4; i++) {
		u[i] = M[i].dotProduct(v);
	}
	return u;
}


Matrix4 Matrix4::operator * (const Matrix4 & N) const {
	Matrix4 R;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				R[i][j] += M[i][k] * N[k][j];
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
				R[i][j] += M[i][k] * N[k][j];
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
		R[i] = M[i] / s;
	}
	return R;
}

void Matrix4::operator /= (const float s) {
	for (int i = 0; i < 4; i++) {
		M[i] /= s;
	}
}


/*
 *	Misc.
 */
Matrix4 Matrix4::transpose() const {
	Matrix4 R;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			R[i][j] = M[j][i];
		}
	}
	return R;
}

Matrix4 Matrix4::inverse () const {

	Matrix4 C;
	float det;

	C[0][0] = M[1][1] * M[2][2] * M[3][3] - 
		M[1][1] * M[2][3] * M[3][2] - 
		M[2][1] * M[1][2] * M[3][3] + 
		M[2][1] * M[1][3] * M[3][2] +
		M[3][1] * M[1][2] * M[2][3] - 
		M[3][1] * M[1][3] * M[2][2];

	C[1][0] = -M[1][0] * M[2][2] * M[3][3] + 
		M[1][0] * M[2][3] * M[3][2] + 
		M[2][0] * M[1][2] * M[3][3] - 
		M[2][0] * M[1][3] * M[3][2] - 
		M[3][0] * M[1][2] * M[2][3] + 
		M[3][0] * M[1][3] * M[2][2];

	C[2][0] = M[1][0] * M[2][1] * M[3][3] -
		M[1][0] * M[2][3] * M[3][1] -
		M[2][0] * M[1][1] * M[3][3] +
		M[2][0] * M[1][3] * M[3][1] +
		M[3][0] * M[1][1] * M[2][3] -
		M[3][0] * M[1][3] * M[2][1];

	C[3][0] = -M[1][0] * M[2][1] * M[3][2] +
		M[1][0] * M[2][2] * M[3][1] +
		M[2][0] * M[1][1] * M[3][2] -
		M[2][0] * M[1][2] * M[3][1] -
		M[3][0] * M[1][1] * M[2][2] +
		M[3][0] * M[1][2] * M[2][1];

	// det must not be 0.
	det = M[0][0] * C[0][0] + 
		M[0][1] * C[1][0] + 
		M[0][2] * C[2][0] + 
		M[0][3] * C[3][0];


	C[0][1] = -M[0][1] * M[2][2] * M[3][3] +
		M[0][1] * M[2][3] * M[3][2] +
		M[2][1] * M[0][2] * M[3][3] -
		M[2][1] * M[0][3] * M[3][2] -
		M[3][1] * M[0][2] * M[2][3] +
		M[3][1] * M[0][3] * M[2][2];

	C[1][1] = M[0][0] * M[2][2] * M[3][3] -
		M[0][0] * M[2][3] * M[3][2] -
		M[2][0] * M[0][2] * M[3][3] +
		M[2][0] * M[0][3] * M[3][2] +
		M[3][0] * M[0][2] * M[2][3] -
		M[3][0] * M[0][3] * M[2][2];

	C[2][1] = -M[0][0] * M[2][1] * M[3][3] +
		M[0][0] * M[2][3] * M[3][1] +
		M[2][0] * M[0][1] * M[3][3] -
		M[2][0] * M[0][3] * M[3][1] -
		M[3][0] * M[0][1] * M[2][3] +
		M[3][0] * M[0][3] * M[2][1];

	C[3][1] = M[0][0] * M[2][1] * M[3][2] -
		M[0][0] * M[2][2] * M[3][1] -
		M[2][0] * M[0][1] * M[3][2] +
		M[2][0] * M[0][2] * M[3][1] +
		M[3][0] * M[0][1] * M[2][2] -
		M[3][0] * M[0][2] * M[2][1];

	C[0][2] = M[0][1] * M[1][2] * M[3][3] -
		M[0][1] * M[1][3] * M[3][2] -
		M[1][1] * M[0][2] * M[3][3] +
		M[1][1] * M[0][3] * M[3][2] +
		M[3][1] * M[0][2] * M[1][3] -
		M[3][1] * M[0][3] * M[1][2];

	C[1][2] = -M[0][0] * M[1][2] * M[3][3] +
		M[0][0] * M[1][3] * M[3][2] +
		M[1][0] * M[0][2] * M[3][3] -
		M[1][0] * M[0][3] * M[3][2] -
		M[3][0] * M[0][2] * M[1][3] +
		M[3][0] * M[0][3] * M[1][2];

	C[2][2] = M[0][0] * M[1][1] * M[3][3] -
		M[0][0] * M[1][3] * M[3][1] -
		M[1][0] * M[0][1] * M[3][3] +
		M[1][0] * M[0][3] * M[3][1] +
		M[3][0] * M[0][1] * M[1][3] -
		M[3][0] * M[0][3] * M[1][1];

	C[3][2] = -M[0][0] * M[1][1] * M[3][2] +
		M[0][0] * M[1][2] * M[3][1] +
		M[1][0] * M[0][1] * M[3][2] -
		M[1][0] * M[0][2] * M[3][1] -
		M[3][0] * M[0][1] * M[1][2] +
		M[3][0] * M[0][2] * M[1][1];

	C[0][3] = -M[0][1] * M[1][2] * M[2][3] +
		M[0][1] * M[1][3] * M[2][2] +
		M[1][1] * M[0][2] * M[2][3] -
		M[1][1] * M[0][3] * M[2][2] -
		M[2][1] * M[0][2] * M[1][3] +
		M[2][1] * M[0][3] * M[1][2];

	C[1][3] = M[0][0] * M[1][2] * M[2][3] -
		M[0][0] * M[1][3] * M[2][2] -
		M[1][0] * M[0][2] * M[2][3] +
		M[1][0] * M[0][3] * M[2][2] +
		M[2][0] * M[0][2] * M[1][3] -
		M[2][0] * M[0][3] * M[1][2];

	C[2][3] = -M[0][0] * M[1][1] * M[2][3] +
		M[0][0] * M[1][3] * M[2][1] +
		M[1][0] * M[0][1] * M[2][3] -
		M[1][0] * M[0][3] * M[2][1] -
		M[2][0] * M[0][1] * M[1][3] +
		M[2][0] * M[0][3] * M[1][1];

	C[3][3] = M[0][0] * M[1][1] * M[2][2] -
		M[0][0] * M[1][2] * M[2][1] -
		M[1][0] * M[0][1] * M[2][2] +
		M[1][0] * M[0][2] * M[2][1] +
		M[2][0] * M[0][1] * M[1][2] -
		M[2][0] * M[0][2] * M[1][1];

	return C / det;
}


/*
 *	Conversion.
 */
Matrix4::operator float * () {
	return * M;
}


/*
 *	Static generators.
 */
Matrix4 Matrix4::identity () {
	Matrix4 M;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				M[i][j] = 1.0;
			} else {
				M[i][j] = 0.0;
			}
		}
	}
	return M;
}

Matrix4 Matrix4::zeroes () {
	Matrix4 M;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			M[i][j] = 0.0;
		}
	}
	return M;
}

Matrix4 Matrix4::diag (float d) {
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
