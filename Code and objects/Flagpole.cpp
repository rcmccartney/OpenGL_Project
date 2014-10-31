#include "Flagpole.h"
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "vec-util.h"

/*
 * The code to make a flagpole (hardcoded vertices) comes from Joe Groff, at his blog 
 *  http://duriansoftware.com/joe/An-intro-to-modern-OpenGL.-Chapter-4:-Rendering-a-Dynamic-3D-Scene-with-Phong-Shading.html
 */
Flagpole::Flagpole(char* filename) : Mesh(filename, NULL, NULL)	 {

	static const GLsizei FLAGPOLE_RES = 16, FLAGPOLE_SLICE = 6;
    GLfloat FLAGPOLE_AXIS_XZ[2] = { -FLAGPOLE_SHAFT_RADIUS, 0.0f };
    static const GLubyte FLAGPOLE_SPECULAR[4] = { 255, 255, 192, 0 };

    static GLfloat
        TEX_FLAGPOLE_LO[2] = { 0.0f,    0.0f },
        TEX_FLAGPOLE_HI[2] = { 0.03125f,  1.0f };

#define _FLAGPOLE_T(y) \
    (TEX_FLAGPOLE_LO[1] \
        + (TEX_FLAGPOLE_HI[1] - TEX_FLAGPOLE_LO[1]) \
        * ((y) - FLAGPOLE_TRUCK_TOP)/(FLAGPOLE_SHAFT_BOTTOM - FLAGPOLE_TRUCK_TOP) \
    )

    GLfloat
        theta_step = 2.0f * (GLfloat)M_PI / (GLfloat)FLAGPOLE_RES,
        s_step = (TEX_FLAGPOLE_HI[0] - TEX_FLAGPOLE_LO[0]) / (GLfloat)FLAGPOLE_RES,
        t_truck_top    = TEX_FLAGPOLE_LO[1],
        t_truck_crown  = _FLAGPOLE_T(FLAGPOLE_TRUCK_CROWN),
        t_truck_bottom = _FLAGPOLE_T(FLAGPOLE_TRUCK_BOTTOM),
        t_shaft_top    = _FLAGPOLE_T(FLAGPOLE_SHAFT_TOP),
        t_shaft_bottom = _FLAGPOLE_T(FLAGPOLE_SHAFT_BOTTOM);

#undef _FLAGPOLE_T

    GLsizei flagpole_vertex_count = 2 + FLAGPOLE_RES * FLAGPOLE_SLICE;
	Mesh::vertex_count = flagpole_vertex_count;
	vertex_data = (struct vertex*) malloc(vertex_count * sizeof(struct vertex));
    
	GLsizei vertex_i = 0, element_i, i;
    GLsizei flagpole_element_count = 3 * ((FLAGPOLE_SLICE - 1) * 2 * FLAGPOLE_RES);
	Mesh::element_count = flagpole_element_count;
	element_data = (GLushort*) malloc(element_count * sizeof(GLushort));
	
	vertex_data[0].position[0] = FLAGPOLE_AXIS_XZ[0];
    vertex_data[0].position[1] = FLAGPOLE_TRUCK_TOP;
    vertex_data[0].position[2] = FLAGPOLE_AXIS_XZ[1];
    vertex_data[0].normal[0]   = 0.0f;
    vertex_data[0].normal[1]   = 1.0f;
    vertex_data[0].normal[2]   = 0.0f;
    vertex_data[0].texcoord[0] = TEX_FLAGPOLE_LO[0];
    vertex_data[0].texcoord[1] = t_truck_top;
    vertex_data[0].shininess   = FLAGPOLE_SHININESS;
    vertex_data[0].specular[0] = 0;
    vertex_data[0].specular[1] = 0;
    vertex_data[0].specular[2] = 0;
    vertex_data[0].specular[3] = 0;

    for (i = 0, vertex_i = 1; i < FLAGPOLE_RES; ++i) {    
		float sn = sinf(theta_step * (float)i), cs = cosf(theta_step * (float)i);
        float s = TEX_FLAGPOLE_LO[0] + s_step * (float)i;
        vertex_data[vertex_i].position[0] = FLAGPOLE_AXIS_XZ[0] + FLAGPOLE_TRUCK_TOP_RADIUS*cs;
        vertex_data[vertex_i].position[1] = FLAGPOLE_TRUCK_TOP;
        vertex_data[vertex_i].position[2] = FLAGPOLE_AXIS_XZ[1] + FLAGPOLE_TRUCK_TOP_RADIUS*sn;
        vertex_data[vertex_i].normal[0]   = cs*0.5f;
        vertex_data[vertex_i].normal[1]   = sqrtf(3.0f/4.0f);
        vertex_data[vertex_i].normal[2]   = sn*0.5f;
        vertex_data[vertex_i].texcoord[0] = s;
        vertex_data[vertex_i].texcoord[1] = t_truck_top;
        vertex_data[vertex_i].shininess   = FLAGPOLE_SHININESS;
        vertex_data[vertex_i].specular[0] = FLAGPOLE_SPECULAR[0];
        vertex_data[vertex_i].specular[1] = FLAGPOLE_SPECULAR[1];
        vertex_data[vertex_i].specular[2] = FLAGPOLE_SPECULAR[2];
        vertex_data[vertex_i].specular[3] = FLAGPOLE_SPECULAR[3];
        ++vertex_i;

        vertex_data[vertex_i].position[0] = FLAGPOLE_AXIS_XZ[0] + FLAGPOLE_TRUCK_CROWN_RADIUS*cs;
        vertex_data[vertex_i].position[1] = FLAGPOLE_TRUCK_CROWN;
        vertex_data[vertex_i].position[2] = FLAGPOLE_AXIS_XZ[1] + FLAGPOLE_TRUCK_CROWN_RADIUS*sn;
        vertex_data[vertex_i].normal[0]   = cs;
        vertex_data[vertex_i].normal[1]   = 0.0f;
        vertex_data[vertex_i].normal[2]   = sn;
        vertex_data[vertex_i].texcoord[0] = s;
        vertex_data[vertex_i].texcoord[1] = t_truck_crown;
        vertex_data[vertex_i].shininess   = FLAGPOLE_SHININESS;
        vertex_data[vertex_i].specular[0] = FLAGPOLE_SPECULAR[0];
        vertex_data[vertex_i].specular[1] = FLAGPOLE_SPECULAR[1];
        vertex_data[vertex_i].specular[2] = FLAGPOLE_SPECULAR[2];
        vertex_data[vertex_i].specular[3] = FLAGPOLE_SPECULAR[3];
        ++vertex_i;

        vertex_data[vertex_i].position[0] = FLAGPOLE_AXIS_XZ[0] + FLAGPOLE_TRUCK_BOTTOM_RADIUS*cs;
        vertex_data[vertex_i].position[1] = FLAGPOLE_TRUCK_BOTTOM;
        vertex_data[vertex_i].position[2] = FLAGPOLE_AXIS_XZ[1] + FLAGPOLE_TRUCK_BOTTOM_RADIUS*sn;
        vertex_data[vertex_i].normal[0]   = cs*sqrtf(15.0f/16.0f);
        vertex_data[vertex_i].normal[1]   = -0.25f;
        vertex_data[vertex_i].normal[2]   = sn*sqrtf(15.0f/16.0f);
        vertex_data[vertex_i].texcoord[0] = s;
        vertex_data[vertex_i].texcoord[1] = t_truck_bottom;
        vertex_data[vertex_i].shininess   = FLAGPOLE_SHININESS;
        vertex_data[vertex_i].specular[0] = FLAGPOLE_SPECULAR[0];
        vertex_data[vertex_i].specular[1] = FLAGPOLE_SPECULAR[1];
        vertex_data[vertex_i].specular[2] = FLAGPOLE_SPECULAR[2];
        vertex_data[vertex_i].specular[3] = FLAGPOLE_SPECULAR[3];
        ++vertex_i;

        vertex_data[vertex_i].position[0] = FLAGPOLE_AXIS_XZ[0] + FLAGPOLE_SHAFT_RADIUS*cs;
        vertex_data[vertex_i].position[1] = FLAGPOLE_SHAFT_TOP;
        vertex_data[vertex_i].position[2] = FLAGPOLE_AXIS_XZ[1] + FLAGPOLE_SHAFT_RADIUS*sn;
        vertex_data[vertex_i].normal[0]   = cs;
        vertex_data[vertex_i].normal[1]   = 0.0f;
        vertex_data[vertex_i].normal[2]   = sn;
        vertex_data[vertex_i].texcoord[0] = s;
        vertex_data[vertex_i].texcoord[1] = t_shaft_top;
        vertex_data[vertex_i].shininess   = FLAGPOLE_SHININESS;
        vertex_data[vertex_i].specular[0] = FLAGPOLE_SPECULAR[0];
        vertex_data[vertex_i].specular[1] = FLAGPOLE_SPECULAR[1];
        vertex_data[vertex_i].specular[2] = FLAGPOLE_SPECULAR[2];
        vertex_data[vertex_i].specular[3] = FLAGPOLE_SPECULAR[3];
        ++vertex_i;

        vertex_data[vertex_i].position[0] = FLAGPOLE_AXIS_XZ[0] + FLAGPOLE_SHAFT_RADIUS*cs;
        vertex_data[vertex_i].position[1] = FLAGPOLE_SHAFT_BOTTOM;
        vertex_data[vertex_i].position[2] = FLAGPOLE_AXIS_XZ[1] + FLAGPOLE_TRUCK_BOTTOM_RADIUS*sn;
        vertex_data[vertex_i].normal[0]   = cs;
        vertex_data[vertex_i].normal[1]   = 0.0f;
        vertex_data[vertex_i].normal[2]   = sn;
        vertex_data[vertex_i].texcoord[0] = s;
        vertex_data[vertex_i].texcoord[1] = t_shaft_bottom;
        vertex_data[vertex_i].shininess   = FLAGPOLE_SHININESS;
        vertex_data[vertex_i].specular[0] = FLAGPOLE_SPECULAR[0];
        vertex_data[vertex_i].specular[1] = FLAGPOLE_SPECULAR[1];
        vertex_data[vertex_i].specular[2] = FLAGPOLE_SPECULAR[2];
        vertex_data[vertex_i].specular[3] = FLAGPOLE_SPECULAR[3];
        ++vertex_i;

        vertex_data[vertex_i].position[0] = FLAGPOLE_AXIS_XZ[0] + FLAGPOLE_SHAFT_RADIUS*cs;
        vertex_data[vertex_i].position[1] = FLAGPOLE_SHAFT_BOTTOM;
        vertex_data[vertex_i].position[2] = FLAGPOLE_AXIS_XZ[1] + FLAGPOLE_TRUCK_BOTTOM_RADIUS*sn;
        vertex_data[vertex_i].normal[0]   =  0.0f;
        vertex_data[vertex_i].normal[1]   = -1.0f;
        vertex_data[vertex_i].normal[2]   =  0.0f;
        vertex_data[vertex_i].texcoord[0] =  s;
        vertex_data[vertex_i].texcoord[1] =  t_shaft_bottom;
        vertex_data[vertex_i].shininess   =  FLAGPOLE_SHININESS;
        vertex_data[vertex_i].specular[0] = FLAGPOLE_SPECULAR[0];
        vertex_data[vertex_i].specular[1] = FLAGPOLE_SPECULAR[1];
        vertex_data[vertex_i].specular[2] = FLAGPOLE_SPECULAR[2];
        vertex_data[vertex_i].specular[3] = FLAGPOLE_SPECULAR[3];
        ++vertex_i;
    }
    vertex_data[vertex_i].position[0] =  0.0f;
    vertex_data[vertex_i].position[1] =  FLAGPOLE_SHAFT_BOTTOM;
    vertex_data[vertex_i].position[2] =  0.0f;
    vertex_data[vertex_i].normal[0]   =  0.0f;
    vertex_data[vertex_i].normal[1]   = -1.0f;
    vertex_data[vertex_i].normal[2]   =  0.0f;
    vertex_data[vertex_i].texcoord[0] =  0.5f;
    vertex_data[vertex_i].texcoord[1] =  t_shaft_bottom;
    vertex_data[vertex_i].shininess   =  FLAGPOLE_SHININESS;
    vertex_data[vertex_i].specular[0] = FLAGPOLE_SPECULAR[0];
    vertex_data[vertex_i].specular[1] = FLAGPOLE_SPECULAR[1];
    vertex_data[vertex_i].specular[2] = FLAGPOLE_SPECULAR[2];
    vertex_data[vertex_i].specular[3] = FLAGPOLE_SPECULAR[3];

    element_i = 0;
    for (i = 0; i < FLAGPOLE_RES - 1; ++i) {
        element_data[element_i++] = 8;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*i;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*(i+1);
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*i;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*i     + 1;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*(i+1);
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*i     + 1;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*(i+1) + 1;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*(i+1);
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*i     + 1;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*i     + 2;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*(i+1) + 1;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*i     + 2;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*(i+1) + 2;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*(i+1) + 1;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*i     + 2;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*i     + 3;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*(i+1) + 2;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*i     + 3;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*(i+1) + 3;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*(i+1) + 2;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*i     + 3;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*i     + 4;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*(i+1) + 3;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*i     + 4;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*(i+1) + 4;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*(i+1) + 3;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*i     + 5;
        element_data[element_i++] = vertex_i;
        element_data[element_i++] = 9 + FLAGPOLE_SLICE*(i+1) + 5;
    }

    element_data[element_i++] = 8;
    element_data[element_i++] = 9 + FLAGPOLE_SLICE*(FLAGPOLE_RES-1);
    element_data[element_i++] = 9;
    element_data[element_i++] = 9 + FLAGPOLE_SLICE*(FLAGPOLE_RES-1);
    element_data[element_i++] = 9 + FLAGPOLE_SLICE*(FLAGPOLE_RES-1) + 1;
    element_data[element_i++] = 9;
    element_data[element_i++] = 9 + FLAGPOLE_SLICE*(FLAGPOLE_RES-1) + 1;
    element_data[element_i++] = 9 + 1;
    element_data[element_i++] = 9;
    element_data[element_i++] = 9 + FLAGPOLE_SLICE*(FLAGPOLE_RES-1) + 1;
    element_data[element_i++] = 9 + FLAGPOLE_SLICE*(FLAGPOLE_RES-1) + 2;
    element_data[element_i++] = 9 + 1;
    element_data[element_i++] = 9 + FLAGPOLE_SLICE*(FLAGPOLE_RES-1) + 2;
    element_data[element_i++] = 9 + 2;
    element_data[element_i++] = 9 + 1;
    element_data[element_i++] = 9 + FLAGPOLE_SLICE*(FLAGPOLE_RES-1) + 2;
    element_data[element_i++] = 9 + FLAGPOLE_SLICE*(FLAGPOLE_RES-1) + 3;
    element_data[element_i++] = 9 + 2;
    element_data[element_i++] = 9 + FLAGPOLE_SLICE*(FLAGPOLE_RES-1) + 3;
    element_data[element_i++] = 9 + 3;
    element_data[element_i++] = 9 + 2;
    element_data[element_i++] = 9 + FLAGPOLE_SLICE*(FLAGPOLE_RES-1) + 3;
    element_data[element_i++] = 9 + FLAGPOLE_SLICE*(FLAGPOLE_RES-1) + 4;
    element_data[element_i++] = 9 + 3;
    element_data[element_i++] = 9 + FLAGPOLE_SLICE*(FLAGPOLE_RES-1) + 4;
    element_data[element_i++] = 9 + 4;
    element_data[element_i++] = 9 + 3;
    element_data[element_i++] = 9 + FLAGPOLE_SLICE*(FLAGPOLE_RES-1) + 5;
    element_data[element_i++] = vertex_i;
    element_data[element_i++] = 9 + 5;

	loadMeshToGPU( GL_STATIC_DRAW );
    free(element_data);
    free(vertex_data);
}

Flagpole::~Flagpole(void)
{
}
