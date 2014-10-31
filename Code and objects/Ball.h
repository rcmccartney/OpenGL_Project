#pragma once
#include "mesh.h"

struct icoVertex
{
	GLfloat position[3];
};

class Ball : public Mesh {
public:
	Ball(char* filename, GLfloat aSpecEx, GLubyte aSpecLight[4], float radius, int slices);
	~Ball(void);
};

