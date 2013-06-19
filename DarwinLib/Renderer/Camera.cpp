
#include "Camera.h"

Camera::Camera () {

	m_Transform  = Matrix4::Identity();

	m_ViewMatrix = Matrix4::Identity();

	m_rotationX = m_rotationY = m_rotationZ = 0.0f;

	m_positionX = m_positionY = m_positionZ = 0.0f;

}

void Camera::SetFrustum (float l, float r,
						 float b, float t,
						 float n, float f) {
										 
	m_Projection = Matrix4::Zeroes();

	float dx = r - l;
	float dy = t - b;
	float dz = f - n;

	m_Projection[0][0] = 2.0f * n / dx;
	m_Projection[1][1] = 2.0f * n / dy;
	m_Projection[2][0] = (r + l) / dx;
	m_Projection[2][1] = (t + b) / dy;
	m_Projection[2][2] = - (f + n) / dz;
	m_Projection[2][3] = -1.0f;
	m_Projection[3][2] = - 2.0f * f * n / dz;
	
}

void Camera::SetOrthografic (float l, float r,
							 float b, float t,
							 float n, float f) {

	m_Projection = Matrix4::Zeroes();
	
	float dx = r - l;
	float dy = t - b;
	float dz = f - n;

	m_Projection[0][0] = 2.0f / dx;
	m_Projection[1][1] = 2.0f / dy;
	m_Projection[2][2] = - 2.0f / dz;
	m_Projection[3][0] = - (r + l) / dx;
	m_Projection[3][1] = - (t + b) / dy;
	m_Projection[3][2] = - (f + n) / dz;
	m_Projection[3][3] = 1.0f;
}

void Camera::SetPerspective (float fovy, float aspect, float near, float far) {

	float tangent = tan(fovy * float(M_PI_2) / 180.0f);
	float height  = near * tangent;
	float width   = height * aspect;

	SetFrustum(-width, width, -height, height, near, far);
}

void Camera::TranslateForward (float dist) {

	m_positionX += dist * sin(m_rotationY);
	m_positionZ -= dist * cos(m_rotationY);
}

void Camera::TranslateRight (float dist) {

	m_positionX += dist * cos(m_rotationY);
	m_positionZ += dist * sin(m_rotationY);
}

void Camera::TranslateUp (float dist) {

	m_positionY += dist;
}

void Camera::RotateRight (float angle) {

	m_rotationY += angle;
}

void Camera::RotateUp (float angle) {

	if (m_rotationX + angle < M_PI_2 &&
		m_rotationX + angle > - M_PI_2) {
		m_rotationX += angle;
	}
}

void Camera::Update () {

	// Rotation on Ox axis.
	float cos_x = cos(m_rotationX);
	float sin_x = sin(m_rotationX);

	Matrix4 RotationX = Matrix4::Identity();
	RotationX[1][1] =  cos_x;
	RotationX[1][2] = -sin_x;
	RotationX[2][1] =  sin_x;
	RotationX[2][2] =  cos_x;


	// Rotation on Oy axis.
	float cos_y = cos(m_rotationY);
	float sin_y = sin(m_rotationY);

	Matrix4 RotationY = Matrix4::Identity();
	RotationY[0][0] =  cos_y;
	RotationY[0][2] =  sin_y;
	RotationY[2][0] = -sin_y;
	RotationY[2][2] =  cos_y;


	// Rotation on Oz axis.
	float cos_z = cos(m_rotationZ);
	float sin_z = sin(m_rotationZ);

	Matrix4 RotationZ = Matrix4::Identity();
	RotationZ[0][0] =  cos_z;
	RotationZ[0][1] = -sin_z;
	RotationZ[1][0] =  sin_z;
	RotationZ[1][1] =  cos_z;


	// Calculate transformation matrix.
	m_Transform = RotationX * RotationY * RotationZ;

	m_Transform[3][0] = m_positionX;
	m_Transform[3][1] = m_positionY;
	m_Transform[3][2] = m_positionZ;

	m_ViewMatrix = m_Transform.Inverse();
}

Matrix4 Camera::ViewMatrix () {

	return m_ViewMatrix;
}

Matrix4 Camera::ProjectionMatrix () {

	return m_Projection;
}

Vector3 Camera::EyePosition () {

	return Vector3(m_positionX, m_positionY, m_positionZ);
}
