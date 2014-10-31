#pragma once
#include "mesh.h"

class GROUND : public Mesh {
public:
	GROUND(char* filename, int textRepeatX, int textRepeatY, GLfloat aSpecEx, 
			GLubyte aSpecLight[4], GLfloat grd_lo[3], GLfloat grd_hi[3]);
	~GROUND(void);
};

