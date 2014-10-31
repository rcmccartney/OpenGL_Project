#pragma once
#include "mesh.h"

class Cone : public Mesh {
public:
	Cone(char* filename, GLfloat aSpecEx, GLubyte aSpecLight[4], float radius, int radialDivisions, int heightDivisions);
	~Cone(void);
};

