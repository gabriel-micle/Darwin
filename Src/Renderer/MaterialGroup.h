
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
	int *	vIndices[3];

	void	addIndexPTN (int & ip, int & it, int & in);
	void	addIndexP	(int & ip);
	void	addIndexPT	(int & ip, int & it);
	void	addIndexPN	(int & ip, int & in);
	void	finalize	();

};

#endif
