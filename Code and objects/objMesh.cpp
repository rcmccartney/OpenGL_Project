#include "objMesh.h"
#include <iostream>
#include <fstream>
using namespace std;

bool objMesh::load_obj(const char* filename, bool hasNormals, bool hasUV) {

	FILE *file = fopen(filename, "r");
	if( file == NULL ){
		printf("Can't open the file. \n");
		return false;
	}
	vector<float> temp_normals, temp_vertices, temp_uvs;
	while( true ) {
		char lineHeader[128];
		//read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if ( strcmp( lineHeader, "v" ) == 0 ) {
			float vx, vy, vz;
			fscanf(file, "%f %f %f\n", &vx, &vy, &vz );
			temp_vertices.push_back(vx);
			temp_vertices.push_back(vy);
			temp_vertices.push_back(vz);
		}
		else if( strcmp( lineHeader, "vt" ) == 0 ) {
			float u, v;
			fscanf(file, "%f %f\n", &u, &v );
			temp_uvs.push_back(u);
			temp_uvs.push_back(v);
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 ) {
			float nx, ny, nz;
			fscanf(file, "%f %f %f\n", &nx, &ny, &nz );
			temp_normals.push_back(nx);
			temp_normals.push_back(ny);
			temp_normals.push_back(nz);
		}
		else if ( strcmp( lineHeader, "f" ) == 0 ){
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			if(hasNormals && hasUV) {
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
										&vertexIndex[0], &uvIndex[0], &normalIndex[0], 
										&vertexIndex[1], &uvIndex[1], &normalIndex[1], 
										&vertexIndex[2], &uvIndex[2], &normalIndex[2] );
				if (matches != 9) {
					printf("Error reading element data in file. Try exporting with other options\n");
					return false;
				}
				elements.push_back(vertexIndex[0]);  //this is an entire vertex of 3 float coordinates
				elements.push_back(vertexIndex[1]);
				elements.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);   //this is a full texture coordinate, 2 floats
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
			else if (hasUV) {
				//Got rid of normal because of issues with Blender
				int matches = fscanf(file, "%d/%d %d/%d %d/%d\n", 
										&vertexIndex[0], &uvIndex[0], 
										&vertexIndex[1], &uvIndex[1], 
										&vertexIndex[2], &uvIndex[2] );  
				if (matches != 6) {
					printf("Error reading element data in file. Try exporting with other options\n");
					return false;
				}
				elements.push_back(vertexIndex[0]);  //this is an entire vertex of 3 float coordinates
				elements.push_back(vertexIndex[1]);
				elements.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);   //this is a full texture coordinate, 2 floats
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
			}
			else if (hasNormals) {
				int matches = fscanf(file, "%d/%d %d/%d %d/%d\n", 
										&vertexIndex[0], &normalIndex[0], 
										&vertexIndex[1], &normalIndex[1], 
										&vertexIndex[2], &normalIndex[2] );  
				if (matches != 6) {
					printf("Error reading element data in file. Try exporting with other options\n");
					return false;
				}
				elements.push_back(vertexIndex[0]);  //this is an entire vertex of 3 float coordinates
				elements.push_back(vertexIndex[1]);
				elements.push_back(vertexIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
			else {
				int matches = fscanf(file, "%d %d %d\n", 
										&vertexIndex[0],  
										&vertexIndex[1],
										&vertexIndex[2] );  
				if (matches != 3) {
					printf("Error reading element data in file. Try exporting with other options\n");
					return false;
				}
				elements.push_back(vertexIndex[0]);  //this is an entire vertex of 3 float coordinates
				elements.push_back(vertexIndex[1]);
				elements.push_back(vertexIndex[2]);
			}
		}
	}
	/*
	for( unsigned int i=0; i < temp_vertices.size(); i++ ) {
		points.push_back( temp_vertices[i] );
	}
	if (hasNormals) {
		for( unsigned int i=0; i < temp_normals.size(); i++ ) {
			normals.push_back( temp_normals[i] );
		}
	}
	if (hasUV) {
		for( unsigned int i=0; i < temp_uvs.size(); i++ ) {
			uv.push_back( temp_uvs[i] );
		}
	}
	*/
	
	//Flatten into a list of all triangle, without using the element_buffer
	for( unsigned int i=0; i < elements.size(); i++ ) {
		unsigned int vertexIndex = elements[i] - 1;  //subtract 1 because obj indexes at 1 not 0
		points.push_back( temp_vertices[vertexIndex*3] );
		points.push_back( temp_vertices[vertexIndex*3+1] );
		points.push_back( temp_vertices[vertexIndex*3+2] );
		if (hasNormals) {
			unsigned int normalIndex = normalIndices[i] - 1;  //subtract 1 because obj indexes at 1 not 0
			normals.push_back( temp_normals[ normalIndex*3 ] );
			normals.push_back( temp_normals[ normalIndex*3+1 ] );
			normals.push_back( temp_normals[ normalIndex*3+2 ] );
		}
		if (hasUV) {
			unsigned int uvIndex = uvIndices[i] - 1;  //subtract 1 because obj indexes at 1 not 0
			uv.push_back( temp_uvs[ uvIndex*2 ] );  //uvs are 2 floats not three
			uv.push_back( temp_uvs[ uvIndex*2+1 ] );
		}
	}

	if (!hasNormals) {
		//add normal info to each face
		for( unsigned int i=0; i+8 < points.size(); i+=9 ) {
			float x0 = points[i];
			float y0 = points[i+1];
			float z0 = points[i+2];
			float x1 = points[i+3];
			float y1 = points[i+4];
			float z1 = points[i+5];
			float x2 = points[i+6];
			float y2 = points[i+7];
			float z2 = points[i+8];
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
			for (int j=0; j < 3; j++) {
				normals.push_back(nx);
				normals.push_back(ny);
				normals.push_back(nz);
			}
		}
	}
	elements.clear(); uvIndices.clear(); normalIndices.clear();
	return true;
}


objMesh::objMesh(char *textName, char *objName, bool hasNormals, bool hasUV, GLfloat aSpecEx, GLubyte aSpecLight[4]) : 
	Mesh(textName, aSpecEx, aSpecLight) {

	if (load_obj(objName, hasNormals, hasUV) ) {
		loadMeshToGPU(GL_STATIC_DRAW);
		free(element_data);
		free(vertex_data);
	}
}


objMesh::~objMesh(void)
{
}
