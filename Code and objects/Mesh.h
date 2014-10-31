#pragma once

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif
#include <vector>
#include "cmatrix"

typedef techsoft::matrix<float> Matrix;

//one vertex is passed with all this info
struct vertex {
	GLfloat position[3];
    GLfloat normal[3];
    GLfloat texcoord[2];
    GLfloat shininess;  //specExonent
    GLubyte specular[4];
};

class Mesh {
public:
	Mesh(char* filename, GLfloat specExponent, GLubyte *specularLight);
	virtual void update_mesh(GLfloat time);
	void loadMeshToGPU( GLenum hint ); 
	void loadTexture(char *filename);
	void convertToArrays();
	GLfloat* get_m_matrix();
	void rotateX(float amount);
	void rotateY(float amount);
	void rotateZ(float amount);
	void scale(float *amounts);
	void translate(float *amounts); 
	//used to add triangles one at a time, dynamic memory management
	void addTriangle(float x0, float y0, float z0, float u0, float v0, 
					 float x1, float y1, float z1, float u1, float v1, 
					 float x2, float y2, float z2, float u2, float v2);
	//The actual mesh and element data to be loaded onto the GPU
	struct vertex *vertex_data;
	GLushort *element_data;
    GLsizei vertex_count, element_count;
	//GPU data locations
	GLuint vertex_buffer, element_buffer, texture;
	GLfloat m_matrix[16];
protected:
	char *fileName;
	GLfloat specExponent;  
    GLubyte specularLight[4];
	std::vector<float> points;
	std::vector<float> normals;
	std::vector<float> uv;
	std::vector<GLushort> elements; 
	Matrix *transformations;
};
