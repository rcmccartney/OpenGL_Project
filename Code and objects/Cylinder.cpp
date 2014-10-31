#include "Cylinder.h"
#define _USE_MATH_DEFINES //needed for M_PI
#include <math.h>

/**
 * makeCylinder - Create polygons for a cylinder with unit height, centered at
 * the origin, with separate number of radial subdivisions and height 
 * subdivisions.
 *
 * @param radius - Radius of the base of the cylinder
 * @param radialDivision - number of subdivisions on the radial base
 * @param heightDivisions - number of subdivisions along the height
 *
 * Can only use calls to addTriangle()
 */
Cylinder::Cylinder(char* filename, GLfloat aSpecEx, GLubyte aSpecLight[4], float radius, int radialDivisions, int heightDivisions) : 
	Mesh(filename, aSpecEx, aSpecLight) {

	float y = -0.5, v = 0;
	float radial_step = 2*M_PI / radialDivisions; 
	float textStep = 1.0 / radialDivisions;
	float step_size = 1.0 / heightDivisions;

	//if radial divisions are 2 it becomes a plane.  At 1, it disappears
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
		//Add CCW dir on top
		addTriangle(0.0, y+1, 0.0, u, v+1,
					x, y+1, z, u, v+1,
					next_x, y+1, next_z, nextU, v+1 );
		//now walk up the side to connect the top and bottom
		for(int j = 0; j < heightDivisions; j++) {
			//bottom triangle
			addTriangle(x, y+j*step_size, z, u, v+j*step_size,
						next_x, y+j*step_size, next_z, nextU, v+j*step_size,
						x, y+(j+1)*step_size, z, u, v+(j+1)*step_size);
			//top triangle
			addTriangle(x, y+(j+1)*step_size, z, u, v+(j+1)*step_size,
						next_x, y+j*step_size, next_z, nextU, v+j*step_size,
						next_x, y+(j+1)*step_size, next_z, nextU, v+(j+1)*step_size);
		}
	}
	loadMeshToGPU(GL_STATIC_DRAW);

}


Cylinder::~Cylinder(void)
{
}
