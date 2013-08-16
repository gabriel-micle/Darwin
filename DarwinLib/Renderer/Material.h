
#pragma once

#include "Math/Math.h"


struct Material {

	Vector4	Ambient;
	Vector4 Diffuse;
	Vector4 Specular;
	float	Shininess;
	int		Id;

	Material () :
		Ambient		(1.0f, 1.0f, 1.0f, 1.0f),
		Diffuse		(1.0f, 1.0f, 1.0f, 1.0f),
		Specular	(1.0f, 1.0f, 1.0f, 1.0f),
		Shininess	(1.0f),
		Id			(0) 
	{}
};
