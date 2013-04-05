
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "..\Math\Math.h"
#include "..\Types.h"

class Material {

public:

				Material ();

private:

	char *		name;		/* Material name. */

	Vector3		Ka;			/* Ambient color. */
	Vector3		Kd;			/* Diffuse color. */
	Vector3		Ks;			/* Specular color. */

	float		alpha;		/* Transparency. */

	float		Ns;			/* Specular shininess. */

	uint_t		illum;		/* Illumination mode. */

	char *		mapKa;		/* Name of the ambient map. */
	char *		mapKd;		/* Name of the diffuse map. */
	char *		mapKs;		/* Name of the specular map. */
	char *		mapBump;	/* Name of the normal map. */

};

#endif
