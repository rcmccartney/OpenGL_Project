#pragma once

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif

class Shader {
public:
	Shader(char *aVert, char *aFrag);
	int remakeShader(GLuint &vertex_shader, GLuint &fragment_shader, GLuint &program);
	void enactShader(GLuint vShader, GLuint fShader, GLuint aProgram); 
	GLuint compileShader(GLuint &vShader, GLuint &fShader);
	void deleteShader();
	void updateShader();
	char* textFileRead(char *fn);
	void show_info_log( GLuint object, PFNGLGETSHADERIVPROC glGet__iv,
						PFNGLGETSHADERINFOLOGPROC glGet__InfoLog );
	~Shader();
	//variables storing position on GPU
	GLuint vertex_shader, fragment_shader, program;
	GLint texture, p_matrix, v_matrix, m_matrix, specCol, diffuseCol, lightPos, light_ambient;
    GLint position, normal, texcoord, shininess, specular;
	char *vert, *frag;
};