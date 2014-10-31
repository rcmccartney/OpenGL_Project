#include "GROUND.h"

GROUND::GROUND(char* filename, int textRepeatX, int textRepeatY, GLfloat aSpecEx, 
				GLubyte aSpecLight[4], GLfloat grd_lo[3], GLfloat grd_hi[3]) : 
	Mesh(filename, NULL, NULL) {

	Mesh::vertex_count = 4;
	vertex_data = (struct vertex*) malloc(vertex_count * sizeof(struct vertex));
    
	Mesh::element_count = 6;
	element_data = (GLushort*) malloc(element_count * sizeof(GLushort));

	//Just four vertices to make a wall, with normal facing up
	vertex_data[0].position[0] = grd_lo[0];
    vertex_data[0].position[1] = grd_lo[1];
    vertex_data[0].position[2] = grd_lo[2];
    vertex_data[0].normal[0]   = 0.0f;
    vertex_data[0].normal[1]   = 1.0f;
    vertex_data[0].normal[2]   = 0.0f;
    vertex_data[0].texcoord[0] = 0;
    vertex_data[0].texcoord[1] = 0;

    vertex_data[1].position[0] = grd_hi[0];
    vertex_data[1].position[1] = grd_lo[1];
    vertex_data[1].position[2] = grd_lo[2];
    vertex_data[1].normal[0]   = 0.0f;
    vertex_data[1].normal[1]   = 1.0f;
    vertex_data[1].normal[2]   = 0.0f;
    vertex_data[1].texcoord[0] = textRepeatX;
    vertex_data[1].texcoord[1] = 0;

    vertex_data[2].position[0] = grd_hi[0];
    vertex_data[2].position[1] = grd_hi[1];
    vertex_data[2].position[2] = grd_hi[2];
    vertex_data[2].normal[0]   = 0.0f;
    vertex_data[2].normal[1]   = 1.0f;
    vertex_data[2].normal[2]   = 0.0f;
	vertex_data[2].texcoord[0] = textRepeatX;
    vertex_data[2].texcoord[1] = textRepeatY;

    vertex_data[3].position[0] = grd_lo[0];
    vertex_data[3].position[1] = grd_hi[1];
    vertex_data[3].position[2] = grd_hi[2];
    vertex_data[3].normal[0]   = 0.0f;
    vertex_data[3].normal[1]   = 1.0f;
    vertex_data[3].normal[2]   = 0.0f;
    vertex_data[3].texcoord[0] = 0;
    vertex_data[3].texcoord[1] = textRepeatY;
	
	for(int i = 0; i < 4; i++) {
		vertex_data[i].shininess = aSpecEx;
		for(int j = 0; j < 4; j++) 
			vertex_data[i].specular[j] = aSpecLight[j];
	}
	
	int element_i = 0;
    element_data[element_i++] = 0;
    element_data[element_i++] = 2;
    element_data[element_i++] = 1;
    element_data[element_i++] = 0;
    element_data[element_i++] = 3;
    element_data[element_i++] = 2;

	loadMeshToGPU( GL_STATIC_DRAW );
    free(element_data);
    free(vertex_data);
}


GROUND::~GROUND(void)
{
}
