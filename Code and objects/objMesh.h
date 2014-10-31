#pragma once
#include "Mesh.h"

class objMesh : public Mesh {
public:
	objMesh(char *textName, char *objName, bool hasNormals, bool hasUV, GLfloat aSpecEx, GLubyte aSpecLight[4]);
	bool load_obj(const char* filename, bool hasNormals, bool hasUV);
	~objMesh();
private:
	std::vector<GLushort> uvIndices, normalIndices;
};

