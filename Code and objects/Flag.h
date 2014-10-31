#pragma once

#include "Mesh.h"

#define FLAG_X_RES 100
#define FLAG_Y_RES 75
#define FLAG_S_STEP (1.0f/((GLfloat)(FLAG_X_RES - 1)))
#define FLAG_T_STEP (1.0f/((GLfloat)(FLAG_Y_RES - 1)))
#define FLAG_VERTEX_COUNT (FLAG_X_RES * FLAG_Y_RES)

class Flag : public Mesh {
public:
	Flag(char *filename);
	virtual void update_mesh(GLfloat time);
	~Flag();
};
