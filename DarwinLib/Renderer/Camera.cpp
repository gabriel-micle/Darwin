
#include "Camera.h"


const Vector3 Camera::WORLD_X_AXIS(1.0f, 0.0f, 0.0f);
const Vector3 Camera::WORLD_Y_AXIS(0.0f, 1.0f, 0.0f);
const Vector3 Camera::WORLD_Z_AXIS(0.0f, 0.0f, 1.0f);

Camera::Camera () : 

	m_ViewMatrix			(Matrix4::IDENTITY),
	m_ProjectionMatrix		(Matrix4::IDENTITY),

	m_eyePosition			(Vector3(0.0f)),
	m_targetPosition		(Vector3(0.0f, 0.0f, -1.0f)),
	m_viewDirection			(Vector3(0.0f, 0.0f, -1.0f)),

	m_absolutePitch			(0.0f),

	m_acceleration			(Vector3(1.0f)),
	m_currentVelocity		(Vector3(0.0f)),
	m_velocity				(Vector3(1.0f)),

	m_zNear					(1.0f),
	m_zFar					(1000.0f),
	m_aspect				(0.0f),
	m_fovY					(60.0f),

	m_xAxis					(Vector3(1.0f, 0.0f, 0.0f)),
	m_yAxis					(Vector3(0.0f, 1.0f, 0.0f)),
	m_zAxis					(Vector3(0.0f, 0.0f, 1.0f))

{}

void Camera::SetFrustum (float l, float r,
						 float b, float t,
						 float n, float f) 
{
	m_ProjectionMatrix = Matrix4();

	float dx = r - l;
	float dy = t - b;
	float dz = f - n;

	m_ProjectionMatrix[0][0] = 2.0f * n / dx;
	m_ProjectionMatrix[1][1] = 2.0f * n / dy;
	m_ProjectionMatrix[2][0] = (r + l) / dx;
	m_ProjectionMatrix[2][1] = (t + b) / dy;
	m_ProjectionMatrix[2][2] = - (f + n) / dz;
	m_ProjectionMatrix[2][3] = -1.0f;
	m_ProjectionMatrix[3][2] = - 2.0f * f * n / dz;

}

void Camera::SetOrthografic (float l, float r,
							 float b, float t,
							 float n, float f) 
{
	m_ProjectionMatrix = Matrix4();

	float dx = r - l;
	float dy = t - b;
	float dz = f - n;

	m_ProjectionMatrix[0][0] = 2.0f / dx;
	m_ProjectionMatrix[1][1] = 2.0f / dy;
	m_ProjectionMatrix[2][2] = - 2.0f / dz;
	m_ProjectionMatrix[3][0] = - (r + l) / dx;
	m_ProjectionMatrix[3][1] = - (t + b) / dy;
	m_ProjectionMatrix[3][2] = - (f + n) / dz;
	m_ProjectionMatrix[3][3] = 1.0f;
}

void Camera::SetPerspective (float fovY, float aspect, float near, float far) {

	float tangent = tan(fovY * static_cast<float>(M_PI_2) / 180.0f);
	float height  = near * tangent;
	float width   = height * aspect;

	SetFrustum(-width, width, -height, height, near, far);

	m_fovY   = fovY;
	m_aspect = aspect;
	m_zNear  = near;
	m_zFar   = far;
}

void Camera::LookAt (const Vector3 & eyePosition, 
					 const Vector3 & targetPosition,
					 const Vector3 & upDirection) 
{
	m_eyePosition    = eyePosition;
	m_targetPosition = targetPosition;

	m_zAxis = eyePosition - targetPosition;
	m_zAxis = m_zAxis.Normalize();

	m_viewDirection = -m_zAxis;

	m_xAxis = upDirection.CrossProduct(m_zAxis);
	m_xAxis = m_xAxis.Normalize();

	m_yAxis = m_zAxis.CrossProduct(m_xAxis);
	m_yAxis = m_yAxis.Normalize();

	m_ViewMatrix[0][0] =  m_xAxis.x;
	m_ViewMatrix[1][0] =  m_xAxis.y;
	m_ViewMatrix[2][0] =  m_xAxis.z;
	m_ViewMatrix[3][0] = -m_xAxis.DotProduct(eyePosition);

	m_ViewMatrix[0][1] =  m_yAxis.x;
	m_ViewMatrix[1][1] =  m_yAxis.y;
	m_ViewMatrix[2][1] =  m_yAxis.z;
	m_ViewMatrix[3][1] = -m_yAxis.DotProduct(eyePosition);

	m_ViewMatrix[0][2] =  m_zAxis.x;
	m_ViewMatrix[1][2] =  m_zAxis.y;
	m_ViewMatrix[2][2] =  m_zAxis.z;    
	m_ViewMatrix[3][2] = -m_zAxis.DotProduct(eyePosition);

	m_absolutePitch = -asin(m_ViewMatrix[1][2]);
}

void Camera::UpdateViewMatrix (bool orthogonolizeAxes) {

	if (orthogonolizeAxes) {

		m_zAxis = m_zAxis.Normalize();

		m_yAxis = m_zAxis.CrossProduct(m_xAxis);
		m_yAxis = m_yAxis.Normalize();

		m_xAxis = m_yAxis.CrossProduct(m_zAxis);
		m_xAxis = m_xAxis.Normalize();

		m_viewDirection = -m_zAxis;
	}

	m_ViewMatrix = Matrix4::IDENTITY;

	m_ViewMatrix[0][0] =  m_xAxis.x;
	m_ViewMatrix[1][0] =  m_xAxis.y;
	m_ViewMatrix[2][0] =  m_xAxis.z;
	m_ViewMatrix[3][0] = -m_xAxis.DotProduct(m_eyePosition);

	m_ViewMatrix[0][1] =  m_yAxis.x;
	m_ViewMatrix[1][1] =  m_yAxis.y;
	m_ViewMatrix[2][1] =  m_yAxis.z;
	m_ViewMatrix[3][1] = -m_yAxis.DotProduct(m_eyePosition);

	m_ViewMatrix[0][2] =  m_zAxis.x;
	m_ViewMatrix[1][2] =  m_zAxis.y;
	m_ViewMatrix[2][2] =  m_zAxis.z;    
	m_ViewMatrix[3][2] = -m_zAxis.DotProduct(m_eyePosition);
}

void Camera::LookAt (const Vector3 & targetPosition) {

	LookAt(m_eyePosition, targetPosition, m_yAxis);
}

void Camera::Translate (float dx, float dy, float dz) {

	Vector3 forwardDirection = WORLD_Y_AXIS.CrossProduct(m_xAxis);
	forwardDirection = forwardDirection.Normalize();

	m_eyePosition += m_xAxis * dx;
	m_eyePosition += WORLD_Y_AXIS * dy;
	m_eyePosition += forwardDirection * dz;

	UpdateViewMatrix(false);
}

void Camera::Translate (const Vector3 & direction, const Vector3 & dist) {

	m_eyePosition += direction * dist;

	UpdateViewMatrix(false);
}

void Camera::Rotate (float yaw, float pitch) {


	float half_pi = static_cast<float>(M_PI_2);

	if (yaw != 0.0f) {

		Matrix4 Rotation = Matrix4::Rotation(WORLD_Y_AXIS, yaw);
		m_xAxis = Rotation * m_xAxis;
		m_zAxis = Rotation * m_zAxis;
	}

	if (pitch != 0.0f) {

		// Add vertical constraints. We don't want to flip upside-down.
		if (m_absolutePitch + pitch <  half_pi &&
			m_absolutePitch + pitch > -half_pi)
		{
			m_absolutePitch += pitch;

			Matrix4 Rotation = Matrix4::Rotation(m_xAxis, pitch);
			m_yAxis = Rotation * m_yAxis;
			m_zAxis = Rotation * m_zAxis;
		}
	}

	UpdateViewMatrix(true);
}


void Camera::UpdatePosition (const Vector3 & direction, float dt) {

	if (m_currentVelocity.Length() != 0.0f) {

		Vector3 disp = m_currentVelocity * dt + 0.5f * m_acceleration * dt * dt;

		if (direction.x == 0.0f && abs(m_currentVelocity.x) < M_EPS) {
			disp.x = 0.0f;
		}

		if (direction.y == 0.0f && abs(m_currentVelocity.y) < M_EPS) {
			disp.y = 0.0f;
		}

		if (direction.z == 0.0f && abs(m_currentVelocity.z) < M_EPS) {
			disp.z = 0.0f;
		}

		Translate(disp.x, disp.y, disp.z);
	}

	UpdateVelocity(direction, dt);
}

void Camera::UpdateVelocity (const Vector3 & direction, float dt) {

	// Updates the camera's velocity based on the supplied movement direction
	// and the elapsed time (since this method was last called). The movement
	// direction is in the range [-1,1].

	if (direction.x != 0.0f) {

		// Camera is moving along the x axis.
		// Linearly accelerate up to the camera's max speed.

		m_currentVelocity.x += direction.x * m_acceleration.x * dt;

		if (m_currentVelocity.x > m_velocity.x) {
			m_currentVelocity.x = m_velocity.x;
		} else if (m_currentVelocity.x < -m_velocity.x) {
			m_currentVelocity.x = -m_velocity.x;
		}

	} else {

		// Camera is no longer moving along the x axis.
		// Linearly decelerate back to stationary state.

		if (m_currentVelocity.x > 0.0f) {
			if ((m_currentVelocity.x -= m_acceleration.x * dt) < 0.0f) {
				m_currentVelocity.x = 0.0f;
			}
		} else {
			if ((m_currentVelocity.x += m_acceleration.x * dt) > 0.0f) {
				m_currentVelocity.x = 0.0f;
			}
		}
	}

	if (direction.y != 0.0f) {

		// Camera is moving along the y axis.
		// Linearly accelerate up to the camera's max speed.

		m_currentVelocity.y += direction.y * m_acceleration.y * dt;

		if (m_currentVelocity.y > m_velocity.y) {
			m_currentVelocity.y = m_velocity.y;
		} else if (m_currentVelocity.y < -m_velocity.y) {
			m_currentVelocity.y = -m_velocity.y;
		}

	} else {

		// Camera is no longer moving along the y axis.
		// Linearly decelerate back to stationary state.

		if (m_currentVelocity.y > 0.0f) {
			if ((m_currentVelocity.y -= m_acceleration.y * dt) < 0.0f) {
				m_currentVelocity.y = 0.0f;
			}
		} else {
			if ((m_currentVelocity.y += m_acceleration.y * dt) > 0.0f) {
				m_currentVelocity.y = 0.0f;
			}
		}
	}

	if (direction.z != 0.0f) {

		// Camera is moving along the z axis.
		// Linearly accelerate up to the camera's max speed.

		m_currentVelocity.z += direction.z * m_acceleration.z * dt;

		if (m_currentVelocity.z > m_velocity.z) {
			m_currentVelocity.z = m_velocity.z;
		} else if (m_currentVelocity.z < -m_velocity.z) {
			m_currentVelocity.z = -m_velocity.z;
		}

	} else {

		// Camera is no longer moving along the z axis.
		// Linearly decelerate back to stationary state.

		if (m_currentVelocity.z > 0.0f) {
			if ((m_currentVelocity.z -= m_acceleration.z * dt) < 0.0f) {
				m_currentVelocity.z = 0.0f;
			}
		} else {
			if ((m_currentVelocity.z += m_acceleration.z * dt) > 0.0f) {
				m_currentVelocity.z = 0.0f;
			}
		}
	}
}
