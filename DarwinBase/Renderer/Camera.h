
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Math\Math.h"

class Camera {

private:

	Matrix4		Transform;
	Matrix4		Projection;

	float		rotationX;
	float		rotationY;
	float		rotationZ;

	float		positionX;
	float		positionY;
	float		positionZ;

public:

				Camera ();

	void		setOrthografic	 (float l, float r, float b, float t, float n, float f);
	void		setFrustum		 (float l, float r, float b, float t, float n, float f);
	void		setPerspective	 (float fovY, float aspect, float zNear, float zFar);

	void		translateForward (float dist);
	void		translateRight   (float dist);
	void		translateUp      (float dist);

	void		rotateRight		 (float angle);
	void		rotateUp		 (float angle);

	Matrix4		viewMatrix		 ();
	Matrix4		projectionMatrix ();

};

#endif

