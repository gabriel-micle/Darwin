
#pragma once


#include "Math\Math.h"


class Camera {

private:

	Matrix4		m_Transform;
	Matrix4		m_Projection;

	float		m_rotationX;
	float		m_rotationY;
	float		m_rotationZ;

	float		m_positionX;
	float		m_positionY;
	float		m_positionZ;

public:

				Camera ();

	void		SetOrthografic	 (float l, float r, float b, float t, float n, float f);
	void		SetFrustum		 (float l, float r, float b, float t, float n, float f);
	void		SetPerspective	 (float fovY, float aspect, float zNear, float zFar);

	void		TranslateForward (float dist);
	void		TranslateRight   (float dist);
	void		TranslateUp      (float dist);

	void		RotateRight		 (float angle);
	void		RotateUp		 (float angle);

	Matrix4		ViewMatrix		 ();
	Matrix4		ProjectionMatrix ();

};
