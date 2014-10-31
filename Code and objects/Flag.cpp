#include "Flag.h"
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
#include "vec-util.h"
#include <iostream>

/*
*  The code to make a flag and make it move comes from Joe Groff, at his blog 
*  http://duriansoftware.com/joe/An-intro-to-modern-OpenGL.-Chapter-4:-Rendering-a-Dynamic-3D-Scene-with-Phong-Shading.html
*/
void calculate_flag_vertex(struct vertex *v, GLfloat s, GLfloat t, GLfloat time) {

    GLfloat sgrad[3] = {
            1.0f + 0.5f*(0.0625f+0.03125f*sinf((GLfloat)M_PI*time))*t*(t - 1.0f),
            0.0f,
            0.125f*(sinf(1.5f*(GLfloat)M_PI*(time + s)) 
					+ s*cosf(1.5f*(GLfloat)M_PI*(time + s))*(1.5f*(GLfloat)M_PI)
            )
        },
        tgrad[3] = {
            -(0.0625f+0.03125f*sinf((GLfloat)M_PI*time))*(1.0f - s)*(2.0f*t - 1.0f),
            0.75f,
            0.0f
        };

    v->position[0] = s - (0.0625f+0.03125f*sinf((GLfloat)M_PI*time))*(1.0f - 0.5f*s)*t*(t-1.0f);
    v->position[1] = 0.75f*t - 0.375f;
    v->position[2] = 0.125f*(s*sinf(1.5f*(GLfloat)M_PI*(time + s)));

    vec_cross(v->normal, tgrad, sgrad);
    vec_normalize(v->normal);

}

Flag::Flag(char *filename) : Mesh(filename, NULL, NULL) {

	Mesh::vertex_count = FLAG_VERTEX_COUNT;
    Mesh::vertex_data = (struct vertex*) malloc(FLAG_VERTEX_COUNT * sizeof(struct vertex));
    Mesh::element_count = 6 * (FLAG_X_RES - 1) * (FLAG_Y_RES - 1);
    Mesh::element_data = (GLushort*) malloc(element_count * sizeof(GLushort));
    GLsizei s, t, i;
    GLushort index;

    for (t = 0, i = 0; t < FLAG_Y_RES; ++t) {
        for (s = 0; s < FLAG_X_RES; ++s, ++i) {
            GLfloat ss = FLAG_S_STEP * s, tt = FLAG_T_STEP * t;
            calculate_flag_vertex(&vertex_data[i], ss, tt, 0.0f);
            vertex_data[i].texcoord[0] = 1-ss;
            vertex_data[i].texcoord[1] = 1-tt;
            vertex_data[i].shininess   = 0.0f;
            vertex_data[i].specular[0] = 0;
            vertex_data[i].specular[1] = 0;
            vertex_data[i].specular[2] = 0;
            vertex_data[i].specular[3] = 0;
        }
	}
    for (t = 0, i = 0, index = 0; t < FLAG_Y_RES - 1; ++t, ++index) {
        for (s = 0; s < FLAG_X_RES - 1; ++s, ++index) {
            element_data[i++] = index             ;
            element_data[i++] = index           +1;
            element_data[i++] = index+FLAG_X_RES  ;
            element_data[i++] = index           +1;
            element_data[i++] = index+FLAG_X_RES+1;
            element_data[i++] = index+FLAG_X_RES  ;
        }
	}
	loadMeshToGPU(GL_STREAM_DRAW);
    free((void*) element_data);
}

void Flag::update_mesh(GLfloat time) {

    GLsizei s, t, i;
    for (t = 0, i = 0; t < FLAG_Y_RES; ++t) {
        for (s = 0; s < FLAG_X_RES; ++s, ++i) {
            GLfloat ss = FLAG_S_STEP * s, tt = FLAG_T_STEP * t;
            calculate_flag_vertex(&Mesh::vertex_data[i], ss, tt, time);
        }
	}

	//don't use loadToGPU because that will regenerate the Buffer location, just update directly
    glBindBuffer(GL_ARRAY_BUFFER, Mesh::vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, FLAG_VERTEX_COUNT * sizeof(struct vertex), Mesh::vertex_data, GL_STREAM_DRAW );
}

Flag::~Flag(void)
{
}
