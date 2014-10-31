#pragma once
#include "mesh.h"
class Cube :
	public Mesh
{
public:
	Cube(char* filename,  GLfloat aSpecEx, GLubyte aSpecLight[4], int subdivisions);
	~Cube();
};

