
#include "Camera.h"

Camera::Camera () {

	Transform  = Matrix4::identity();

	rotationX = rotationY = rotationZ = 0.0f;

	positionX = positionY = positionZ = 0.0f;

}

void Camera::setFrustum (float l, float r,
						 float b, float t,
						 float n, float f) {
										 
	Projection = Matrix4::zeroes();

	float dx = r - l;
	float dy = t - b;
	float dz = f - n;

	Projection[0][0] = 2.0f * n / dx;
	Projection[1][1] = 2.0f * n / dy;
	Projection[2][0] = (r + l) / dx;
	Projection[2][1] = (t + b) / dy;
	Projection[2][2] = - (f + n) / dz;
	Projection[2][3] = -1.0f;
	Projection[3][2] = - 2.0f * f * n / dz;
	
}

void Camera::setOrthografic (float l, float r,
							 float b, float t,
							 float n, float f) {

	Projection = Matrix4::zeroes();
	
	float dx = r - l;
	float dy = t - b;
	float dz = f - n;

	Projection[0][0] = 2.0f / dx;
	Projection[1][1] = 2.0f / dy;
	Projection[2][2] = - 2.0f / dz;
	Projection[3][0] = - (r + l) / dx;
	Projection[3][1] = - (t + b) / dy;
	Projection[3][2] = - (f + n) / dz;
	Projection[3][3] = 1.0f;
}

void Camera::setPerspective (float fovy, float aspect, float near, float far) {

	float tangent = tan(fovy * M_PI_2 / 180.0f);
	float height  = near * tangent;
	float width   = height * aspect;

	setFrustum(-width, width, -height, height, near, far);
}

void Camera::translateForward (float dist) {

	positionX += dist * sin(rotationY);
	positionZ -= dist * cos(rotationY);
}

void Camera::translateRight (float dist) {

	positionX += dist * cos(rotationY);
	positionZ += dist * sin(rotationY);
}

void Camera::translateUp (float dist) {

	positionY += dist;
}

void Camera::rotateRight (float angle) {

	rotationY += angle;
}

void Camera::rotateUp (float angle) {

	if (rotationX + angle < M_PI_2 &&
		rotationX + angle > - M_PI_2) {
		rotationX += angle;
	}
}

Matrix4 Camera::viewMatrix () {

	// Rotation on Ox axis.
	float cos_x = cos(rotationX);
	float sin_x = sin(rotationX);

	Matrix4 RotationX = Matrix4::identity();
	RotationX[1][1] =  cos_x;
	RotationX[1][2] = -sin_x;
	RotationX[2][1] =  sin_x;
	RotationX[2][2] =  cos_x;


	// Rotation on Oy axis.
	float cos_y = cos(rotationY);
	float sin_y = sin(rotationY);

	Matrix4 RotationY = Matrix4::identity();
	RotationY[0][0] =  cos_y;
	RotationY[0][2] =  sin_y;
	RotationY[2][0] = -sin_y;
	RotationY[2][2] =  cos_y;


	// Rotation on Oz axis.
	float cos_z = cos(rotationZ);
	float sin_z = sin(rotationZ);

	Matrix4 RotationZ = Matrix4::identity();
	RotationZ[0][0] =  cos_z;
	RotationZ[0][1] = -sin_z;
	RotationZ[1][0] =  sin_z;
	RotationZ[1][1] =  cos_z;


	// Calculate transformation matrix.
	Transform = RotationX * RotationY * RotationZ;

	Transform[3][0] = positionX;
	Transform[3][1] = positionY;
	Transform[3][2] = positionZ;

	return Transform.inverse();
}

Matrix4 Camera::projectionMatrix () {

	return Projection;
}
