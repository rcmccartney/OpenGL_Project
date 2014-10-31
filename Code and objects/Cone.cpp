#include "Cone.h"
#define _USE_MATH_DEFINES //needed for M_PI
#include <math.h>

/**
 * makeCone - Create polygons for a cone with unit height, centered at the
 * origin, with separate number of radial subdivisions and height 
 * subdivisions
 *
 * @param radius - Radius of the base of the cone
 * @param radialDivision - number of subdivisions on the radial base
 * @param heightDivisions - number of subdivisions along the height
 *
 * Can only use calls to addTriangle()
 */
Cone::Cone(char* filename, GLfloat aSpecEx, GLubyte aSpecLight[4], float radius, int radialDivisions, int heightDivisions) : 
	Mesh(filename, aSpecEx, aSpecLight) {

	float y = -0.5, v=0;
	float radial_step = 2*M_PI / radialDivisions; 
	float step_size = 1.0 / heightDivisions;
	float textStep = 1.0 / radialDivisions;

	for(int i = 0; i < radialDivisions; i++) {
		//Add CW direction on the bottom
		float x = radius*cos(i*radial_step);
		float next_x = radius*cos((i+1)*radial_step);
		float z = radius*sin(i*radial_step);
		float next_z =  radius*sin((i+1)*radial_step);
		float u = i*textStep; 
		float nextU = (i+1)*textStep;

		addTriangle(0.0, y, 0.0, u, v,
					next_x, y, next_z, nextU, v,
					x, y, z, u, v);
		//now walk up the side to connect the top and bottom
		//here, x & z are reduced each time to the center point while y increases
		for(int j = 0; j < heightDivisions; j++) {
			//bottom triangle
			addTriangle(x-j*x/heightDivisions, y+j*step_size, z-j*z/heightDivisions, u, v+j*step_size,
						next_x-j*next_x/heightDivisions, y+j*step_size, next_z-j*next_z/heightDivisions, nextU, v+j*step_size,
						x-(j+1)*x/heightDivisions, y+(j+1)*step_size, z-(j+1)*z/heightDivisions, u, v+(j+1)*step_size);
			//top triangle
			addTriangle(x-(j+1)*x/heightDivisions, y+(j+1)*step_size, z-(j+1)*z/heightDivisions, u, v+(j+1)*step_size,
						next_x-j*next_x/heightDivisions, y+j*step_size, next_z-j*next_z/heightDivisions, nextU, v+j*step_size,
						next_x-(j+1)*next_x/heightDivisions, y+(j+1)*step_size, next_z-(j+1)*next_z/heightDivisions, nextU, v+(j+1)*step_size);
		}
	}
	loadMeshToGPU(GL_STATIC_DRAW);
}

Cone::~Cone(void)
{
}
