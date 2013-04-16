
#ifndef _MATERIAL_GROUP_H_
#define _MATERIAL_GROUP_H_

// Container for polygons that have the same material ID.
class MaterialGroup {

public:

			MaterialGroup ();
			MaterialGroup (const char * name);

	char	materialName[255];

	int		cIndices;
	int		nIndices;
	int *	vIndices;

	void	addIndex (int & idx);
	void	finalize ();

};

#endif
