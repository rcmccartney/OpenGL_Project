#include "Mesh.h"
#include <iostream>
#include <SOIL.h>
#define _USE_MATH_DEFINES //needed for M_PI
#include <math.h>

Mesh::Mesh(char *filename, GLfloat aSpecEx, GLubyte aSpecLight[4]) {

	fileName = filename;
	if (aSpecLight != NULL) { 
		specExponent = aSpecEx;
		for(int i = 0; i < 4; i++) 
			specularLight[i] = aSpecLight[i];
	}
	loadTexture(filename);
	if (texture == 0) {
		std::cerr << "Failed to load texture." << std::endl;
	}

	transformations = new Matrix(4,4,0.0);
	for(int i = 0; i < 4; i++) {
		(*transformations)[i][i] = 1.0;  //make it a diagonal matrix
	}
}

GLfloat* Mesh::get_m_matrix() {

	for( int i = 0; i < 16; i++) {
		int j = i / 4;  //row
		int k = i % 4;  //col
		m_matrix[i] = (*transformations)[k][j];  //openGL uses col order
	}
	return m_matrix;
}

void Mesh::rotateX(float amount) {

	// Compute the sines and cosines of each rotation
    // about each axis
    float cx = cos(amount * M_PI / 180);
    float sx = sin(amount * M_PI / 180);
	Matrix rotationx(4,4,0.0);
	rotationx[0][0] = 1.0; 
	rotationx[1][1] = cx;
	rotationx[1][2] = -sx;
	rotationx[2][1] = sx;
	rotationx[2][2] = cx;
	rotationx[3][3] = 1.0;

	*transformations = rotationx * (*transformations);
}

void Mesh::rotateY(float amount) {

    float cy = cos(amount * M_PI / 180);
    float sy = sin(amount * M_PI / 180);
	Matrix rotationy(4,4,0.0);
	rotationy[0][0] = cy;
	rotationy[0][2] = sy;
	rotationy[1][1] = 1.0;
	rotationy[2][0] = -sy;
	rotationy[2][2] = cy;
	rotationy[3][3] = 1.0;

	*transformations = rotationy * (*transformations);
}

void Mesh::rotateZ(float amount) {

    float cz = cos(amount * M_PI / 180);
    float sz = sin(amount * M_PI / 180);
	Matrix rotationz(4,4,0.0);
	rotationz[0][0] = cz;
	rotationz[0][1] = -sz;
	rotationz[1][0] = sz;
	rotationz[1][1] = cz;
	rotationz[2][2] = 1.0;
	rotationz[3][3] = 1.0;

	*transformations = rotationz * (*transformations);
}

/*
 * Amount should be a non-zero factor for {x,y,z}
 */
void Mesh::scale(float *amounts) {
	Matrix scale(4,4,0.0);
	scale[0][0] = amounts[0];
	scale[1][1] = amounts[1];
	scale[2][2] = amounts[2];
	scale[3][3] = 1.0;

	*transformations = scale * (*transformations);
}

void Mesh::translate(float *amounts) {
	Matrix trans(4,4,0.0);
	trans[0][0] = 1.0;
	trans[1][1] = 1.0;
	trans[2][2] = 1.0;
	trans[3][3] = 1.0;
	for(int i = 0; i < 3; i++) {
		trans[i][3] = amounts[i]; 
	}
	*transformations = trans * (*transformations);
}

void Mesh::convertToArrays() {

	vertex_count = points.size() / 3;  //every 3 floats is a vertex
	vertex_data = (struct vertex*) malloc(vertex_count * sizeof(struct vertex) );
	if (elements.size() == 0) {
		element_count = vertex_count; //elements are numbered in order of addTriangle
		element_data = (GLushort*) malloc(element_count * sizeof(GLushort));
	}
	else {
		element_count = elements.size();
		element_data = (GLushort*) malloc(element_count * sizeof(GLushort));
		for(int i = 0; i < element_count; i++) {
			element_data[i] = elements[i];
		}
	}
	
    for (int i=0; i < vertex_count; i++) {
		vertex_data[i].position[0] = points[i*3];
		vertex_data[i].position[1] = points[i*3+1];
		vertex_data[i].position[2] = points[i*3+2];

		if (i*3+2 < normals.size() ) { 
			vertex_data[i].normal[0] = normals[i*3];
			vertex_data[i].normal[1] = normals[i*3+1];
			vertex_data[i].normal[2] = normals[i*3+2];
		}
		if (uv.size() != 0) {
			vertex_data[i].texcoord[0] = uv[i*2];
			vertex_data[i].texcoord[1] = uv[i*2+1];
		}
		vertex_data[i].shininess = specExponent;
		for(int j=0; j < 4; j++) {
			vertex_data[i].specular[j] = specularLight[j];
		}
		if (elements.size() == 0) {
			element_data[i] = i;
		}
    }
}

void Mesh::loadMeshToGPU( GLenum hint )  {

	//if you used dynamic memory allocation, now you need to convert into the vertex and element buffers
	if (points.size() > 0) {
		convertToArrays();
	}

	glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &element_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(vertex), vertex_data, hint);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_count*sizeof(GLushort), element_data, GL_STATIC_DRAW );
}

//empty, but used by children polymorphically
void Mesh::update_mesh(GLfloat time) { }

/**
 * adds a triangle to the current shape
 */
void Mesh::addTriangle (float x0, float y0, float z0, float u0, float v0, 
						float x1, float y1, float z1, float u1, float v1, 
						float x2, float y2, float z2, float u2, float v2) {

    points.push_back (x0);
    points.push_back (y0);
    points.push_back (z0);
    
    points.push_back (x1);
    points.push_back (y1);
    points.push_back (z1);
    
    points.push_back (x2);
    points.push_back (y2);
    points.push_back (z2);
    
    // calculate the normal
    float ux = x1 - x0;
    float uy = y1 - y0;
    float uz = z1 - z0;
    float vx = x2 - x0;
    float vy = y2 - y0;
    float vz = z2 - z0;   
    float nx = (uy * vz) - (uz * vy);
    float ny = (uz * vx) - (ux * vz);
    float nz = (ux * vy) - (uy * vx);
    
    // Attach the normal to all 3 vertices
    for (int i=0; i < 3; i++) {
        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(nz);
    }
    
    // Attach the texture coords
    uv.push_back(u0);
    uv.push_back(v0);
    uv.push_back(u1);
    uv.push_back(v1);
    uv.push_back(u2);
    uv.push_back(v2);
}

void Mesh::loadTexture(char *filename) {

	int width, height;
	unsigned char* image;
	image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

	glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,			/* target, level */
				GL_RGB,						/* internal format */
				width, height, 0,			/* width, height, border */
				GL_RGB, GL_UNSIGNED_BYTE,	/* external format, type */
				image );
	SOIL_free_image_data(image);

	/* load an image file directly as a new OpenGL texture */
	/*
	tex_2d[0] = SOIL_load_OGL_texture( "texture.jpg",
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID, 
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS);
	if ( 0 == tex_2d[0] ) { 
		printf( "SOIL loading error: '%s'\n",SOIL_last_result() );
	}
	*/
}