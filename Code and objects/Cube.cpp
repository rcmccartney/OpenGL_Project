#include "Cube.h"

/**
 * makeCube - Create a unit cube, centered at the origin, with a given number
 * of subdivisions in each direction on each face.
 *
 * @param subdivision - number of equal subdivisons to be made in each 
 *        direction along each face
 *
 * Can only use calls to addTriangle()
 */
Cube::Cube(char* filename, GLfloat aSpecEx, GLubyte aSpecLight[4], int subdivisions) : 
	Mesh(filename, aSpecEx, aSpecLight) {

	float step_size = 1.0 / subdivisions;
	float x = -0.5, y = -0.5, z = -0.5;
	float u = 0, v = 0;

	for(int i = 0; i < subdivisions; i++) {
		for(int j = 0; j < subdivisions; j++) {
			//makes the bottom triangle as we walk left to right
			addTriangle(x+j*step_size, y+i*step_size, z, u+j*step_size, v+i*step_size,
						x+(j+1)*step_size, y+i*step_size, z, u+(j+1)*step_size, v+i*step_size,
						x+(j+1)*step_size, y+(i+1)*step_size, z, u+(j+1)*step_size, v+(i+1)*step_size);
			//makes the top triangle as we walk left to right
			addTriangle(x+j*step_size, y+i*step_size, z, u+j*step_size, v+i*step_size,
						x+(j+1)*step_size, y+(i+1)*step_size, z, u+(j+1)*step_size, v+(i+1)*step_size,
						x+j*step_size, y+(i+1)*step_size, z, u+j*step_size, v+(i+1)*step_size);
			//Now do the same thing for the back face (z+1)
			//Need to change to CW direction
			addTriangle(x+j*step_size, y+i*step_size, z+1, u+j*step_size, v+i*step_size,
					    x+(j+1)*step_size, y+(i+1)*step_size, z+1, u+(j+1)*step_size, v+(i+1)*step_size,
						x+(j+1)*step_size, y+i*step_size, z+1, u+(j+1)*step_size, v+i*step_size);
			addTriangle(x+j*step_size, y+i*step_size, z+1, u+j*step_size, v+i*step_size,
						x+j*step_size, y+(i+1)*step_size, z+1, u+j*step_size, v+(i+1)*step_size,
						x+(j+1)*step_size, y+(i+1)*step_size, z+1, u+(j+1)*step_size, v+(i+1)*step_size);
			
			//Now repeat the same calculations for the X-Z bottom plane
			//Using right hand rule, the bottom is CW
			addTriangle(x+j*step_size, y, z+i*step_size, u+j*step_size, v+i*step_size,
						x+(j+1)*step_size, y, z+(i+1)*step_size, u+(j+1)*step_size, v+(i+1)*step_size,
						x+(j+1)*step_size, y, z+i*step_size, u+(j+1)*step_size, v+i*step_size);
			addTriangle(x+j*step_size, y, z+i*step_size, u+j*step_size, v+i*step_size,
						x+j*step_size, y, z+(i+1)*step_size, u+j*step_size, v+(i+1)*step_size,
						x+(j+1)*step_size, y, z+(i+1)*step_size, u+(j+1)*step_size, v+(i+1)*step_size);
			//X-Z top plane, CCW using right-hand rule
			addTriangle(x+j*step_size, y+1, z+i*step_size, u+j*step_size, v+i*step_size,
						x+(j+1)*step_size, y+1, z+i*step_size, u+(j+1)*step_size, v+i*step_size,
						x+(j+1)*step_size, y+1, z+(i+1)*step_size, u+(j+1)*step_size, v+(i+1)*step_size);
			addTriangle(x+j*step_size, y+1, z+i*step_size, u+j*step_size, v+i*step_size,
						x+(j+1)*step_size, y+1, z+(i+1)*step_size, u+(j+1)*step_size, v+(i+1)*step_size,
						x+j*step_size, y+1, z+(i+1)*step_size, u+j*step_size, v+(i+1)*step_size);
			
			//Now do the same thing for the Y_Z plane, left first CCW
			addTriangle(x, y+j*step_size, z+i*step_size, u+j*step_size, v+i*step_size,
						x, y+(j+1)*step_size, z+i*step_size, u+(j+1)*step_size, v+i*step_size,
						x, y+(j+1)*step_size, z+(i+1)*step_size, u+(j+1)*step_size, v+(i+1)*step_size);
			addTriangle(x, y+j*step_size, z+i*step_size, u+j*step_size, v+i*step_size,
						x, y+(j+1)*step_size, z+(i+1)*step_size, u+(j+1)*step_size, v+(i+1)*step_size,
						x, y+j*step_size, z+(i+1)*step_size, u+j*step_size, v+(i+1)*step_size);
			//Y-Z right plane, CW using right-hand rule
			addTriangle(x+1, y+j*step_size, z+i*step_size, u+j*step_size, v+i*step_size,
						x+1, y+(j+1)*step_size, z+(i+1)*step_size, u+(j+1)*step_size, v+(i+1)*step_size,
						x+1, y+(j+1)*step_size, z+i*step_size, u+(j+1)*step_size, v+i*step_size);
			addTriangle(x+1, y+j*step_size, z+i*step_size, u+j*step_size, v+i*step_size,
						x+1, y+j*step_size, z+(i+1)*step_size, u+j*step_size, v+(i+1)*step_size,
						x+1, y+(j+1)*step_size, z+(i+1)*step_size, u+(j+1)*step_size, v+(i+1)*step_size);
		}
	}
	loadMeshToGPU(GL_STATIC_DRAW);
}

Cube::~Cube(void)
{
}
