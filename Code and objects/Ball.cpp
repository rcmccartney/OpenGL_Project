#include "Ball.h"
#include <math.h>
#include "vec-util.h"
#define _USE_MATH_DEFINES //needed for M_PI
#include <deque>
using namespace std;

//this is the icosahedron vertices
float a = 2 / (1 + sqrt(5.0) );
const icoVertex vertices[12] =
{
	{ { 0.0, a, -1.0} }, 
	{ { -a, 1.0, 0.0} },
	{ { a, 1.0, 0.0} },
	{ { 0.0, a, 1.0} },
	{ { -1.0, 0.0, a} },
	{ { 0.0, -a, 1.0} },
	{ { 1.0, 0.0, a } },
	{ { 1.0, 0.0, -a } },
	{ { 0.0, -a, -1.0 } },
	{ { -1.0, 0.0, -a } },
	{ { -a, -1.0, 0.0} },
	{ { a, -1.0, 0.0 } }
};

//every three indices represent a triangle in the icosahedron
const int ICO_SIZE = 60;
const int indices[ICO_SIZE] =
{
	0,1,2,  3,2,1,  3,4,5,   3,5,6,
	0,7,8,  0,8,9,  5,10,11, 8,11,10,
	1,9,4,  10,4,9, 2,6,7,   11,7,6,
	3,1,4,	3,6,2,  0,9,1,   0,2,7,
	8,10,9, 8,7,11, 5,4,10,  5,11,6
};

//this function averages two vertices to make a new vertex that is the midpoint
icoVertex average(icoVertex &a, icoVertex &b) {
	icoVertex c;
	c.position[0] = ( a.position[0] + b.position[0] ) / 2;
	c.position[1] = ( a.position[1] + b.position[1] ) / 2;
	c.position[2] = ( a.position[2] + b.position[2] ) / 2;
	return c;
}

//recursively add triangles to the sphere
void recursive_sphere(deque<icoVertex> &verts, int recursion) {
	//base case, stop subdividing triangles
	if (recursion <= 1) {
		return;
	}
	else {
		icoVertex a, b, c, a_prime, b_prime, c_prime;
		//vertices are in groups of 3.  Take each one off, subdivide it,
		//then add it to the back of the vertex vector.  
		//like a conveyor belt going around
		int iterations = verts.size() / 3;
		for(int i = 0; i < iterations; i++) {
			a = verts.front(); verts.pop_front();
			b = verts.front(); verts.pop_front();
			c = verts.front(); verts.pop_front();
			a_prime = average(a, b);
			b_prime = average(b, c);
			c_prime = average(c, a);
			//now add the 4 new triangles formed
			verts.push_back(a); verts.push_back(a_prime); verts.push_back(c_prime);
			verts.push_back(a_prime); verts.push_back(b); verts.push_back(b_prime);
			verts.push_back(c_prime); verts.push_back(a_prime); verts.push_back(b_prime);
			verts.push_back(c_prime); verts.push_back(b_prime); verts.push_back(c);
		}
		//keep going by splitting the triangle more
		recursive_sphere(verts, --recursion);
	}
}

/**
 *  Uses recursive subdivision starting with an icosahedron
 *  and each triangle is subdivided into 4 triangles
 *  Then normalize all the vertices and scale by the radius
 */
Ball::Ball(char* filename, GLfloat aSpecEx, GLubyte aSpecLight[4], float radius, int slices) : 
	Mesh(filename, aSpecEx, aSpecLight) {

	deque<icoVertex> new_vertices;
	//initialize with the icosahedron vertices
	for(int i = 0; i < ICO_SIZE; i++) {
		new_vertices.push_back( vertices[ indices[i] ] );
	}
	//don't use stacks for this implementation, just slices
	//here slices is the number of recursive calls to make
	if (slices > 1) {
		//recursively add triangles
		recursive_sphere(new_vertices, slices);
		//normalize every vertex to push it back to the edge of the sphere
		for(int i = 0; i < new_vertices.size(); i++) {
			vec_normalize( new_vertices[i].position );
		}
	} //else just print the normal icosahedron

	//Now, just add a triangle for each set of three vertices 
	//It is normalized, so scale by the radius to resize each triangle
	//since we added triangles in groups of 3, new_vertices is divisible by 3
	int iterations = new_vertices.size() / 3;
	for(int i = 0; i < iterations ; i++) {
		int j = i*3;
		//U and V are the texture coordinates, mapped using speherical coordinates
		float v1 = 1-acos( new_vertices[j].position[1] ) / M_PI;
		float v2 = 1-acos( new_vertices[j+1].position[1] ) / M_PI;
		float v3 = 1-acos( new_vertices[j+2].position[1] ) / M_PI;
		float u1 = ( atan2(new_vertices[j].position[0], new_vertices[j].position[2]) + M_PI) / (2*M_PI);
		float u2 = ( atan2(new_vertices[j+1].position[0], new_vertices[j+1].position[2]) + M_PI) / (2*M_PI)  ;
		float u3 = ( atan2(new_vertices[j+2].position[0], new_vertices[j+2].position[2]) + M_PI) / (2*M_PI) ;

		addTriangle(radius*new_vertices[j].position[0], radius*new_vertices[j].position[1], radius*new_vertices[j].position[2], u1, v1,
			radius*new_vertices[j+1].position[0], radius*new_vertices[j+1].position[1], radius*new_vertices[j+1].position[2], u2, v2,
			radius*new_vertices[j+2].position[0], radius*new_vertices[j+2].position[1], radius*new_vertices[j+2].position[2], u3, v3 );
	}

	loadMeshToGPU(GL_STREAM_DRAW);
}

Ball::~Ball(void)
{
}
