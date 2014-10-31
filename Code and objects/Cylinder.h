#pragma once
#include "mesh.h"
class Cylinder :
	public Mesh
{
public:
	Cylinder(char* filename,  GLfloat aSpecEx, GLubyte aSpecLight[4], float radius, int radialDivisions, int heightDivisions);
	~Cylinder();
};

