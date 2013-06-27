
#pragma once


#include "Math/Math.h"


class Camera {

// Public members.
public:

	static const Vector3 WORLD_X_AXIS;
	static const Vector3 WORLD_Y_AXIS;
	static const Vector3 WORLD_Z_AXIS;


// Private members.
private:

	Matrix4		m_ViewMatrix;
	Matrix4		m_ProjectionMatrix;

	Vector3		m_eyePosition;
	Vector3		m_targetPosition;
	Vector3		m_viewDirection;

	float		m_absolutePitch;
	
	Vector3		m_acceleration;
	Vector3		m_currentVelocity;
	Vector3		m_velocity;

	float		m_zNear;
	float		m_zFar;
	float		m_aspect;
	float		m_fovY;

	Vector3		m_xAxis;
	Vector3		m_yAxis;
	Vector3		m_zAxis;


// Public methods.
public:

				Camera				();

	void		SetOrthografic		(float l, float r, float b, float t, float n, float f);
	void		SetFrustum			(float l, float r, float b, float t, float n, float f);
	void		SetPerspective		(float fovY, float aspect, float zNear, float zFar);

	void		Rotate				(float yaw, float pitch);
	void		Translate			(const Vector3 & direction, const Vector3 & dist);
	void		Translate			(float dx, float dy, float dz);

	void		LookAt				(const Vector3 & targetPosition);
	void		LookAt				(const Vector3 & eyePosition, const Vector3 & targetPosition, const Vector3 & upDirection);

	void		UpdateViewMatrix	(bool orthogonalizeAxes);
	void		UpdatePosition		(const Vector3 & direction, float dt);
	void		UpdateVelocity		(const Vector3 & direction, float dt);

	Matrix4		GetViewMatrix		() const { return m_ViewMatrix; }
	Matrix4		GetProjectionMatrix () const { return m_ProjectionMatrix; }
	Vector3		GetEyePosition		() const { return m_eyePosition; }

	Vector3		GetPosition			() const { return m_eyePosition; }
	Vector3		GetAcceleration		() const { return m_acceleration; }
	Vector3		GetVelocity			() const { return m_velocity; }
	Vector3		GetCurrentVelocity	() const { return m_currentVelocity; }

	void		SetPosition			(const Vector3 & p) { m_eyePosition = p; }
	void		SetAcceleration		(const Vector3 & a) { m_acceleration = a; }
	void		SetVelocity			(const Vector3 & v) { m_velocity = v; }
	void		SetCurrentVelocity	(const Vector3 & v) { m_currentVelocity = v; }

};
