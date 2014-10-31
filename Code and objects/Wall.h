#pragma once
#include "mesh.h"

class Wall : public Mesh {
public:
	Wall(char* filename, int textRepeatX, int textRepeatY, GLfloat aSpecEx, 
		GLubyte aSpecLight[4], GLfloat wall_lo[3], GLfloat wall_hi[3]);
	~Wall(void);
};

