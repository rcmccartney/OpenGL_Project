#pragma once

#include "Shader.h"
#include "Mesh.h"
#include <deque>

#define INITIAL_WIDTH  640
#define INITIAL_HEIGHT 480

static float PROJECTION_TOP = 1.0f;
static float PROJECTION_BOTTOM = -1.0f;
static float PROJECTION_LEFT = (INITIAL_WIDTH/INITIAL_HEIGHT)*PROJECTION_BOTTOM;
static float PROJECTION_RIGHT = (INITIAL_WIDTH/INITIAL_HEIGHT)*PROJECTION_TOP;
static float PROJECTION_NEAR = 1.0f;
static float PROJECTION_FAR = 40.0f;
static float ANGLEINC = 5.0f;
static float STEPSIZE = 0.05f;
static float AMB_INC = 0.01f;

static GLfloat lightpos[4] = {5, 7, -1, 1};
static GLfloat specularcolor[4] = {1.0, 1.0, 1.0, 1.0};
static GLfloat diffusecolor[4] = {1.0, 1.0, 1.0, 1.0};

class Window {
public:
	Window(Shader *aShader, int argc, char* argv[] );
	void keyboard(unsigned char key, int x, int y );
	void render();
	void update();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void changeAmbience(float amount);
	void render_mesh();
	void reshape(int w, int h);
	void dragMouse(int x, int y);
	void dropMouse(int button, int state, int x, int y);
	void addMesh(Mesh *aMesh);
	void rotateX(float amount);
	void rotateY(float amount);
	void rotateZ(float amount);
	void setUpCamera();
	void setUpFrustrum();
	void setUpOrtho();
	~Window();
private:
	void setupCallback();
	Shader *shader;
	GLsizei window_size[2];
	GLfloat ambientcolor[4];
	GLfloat p_matrix[16], v_matrix[16];
	GLfloat up[3], eye[3], look[3];
	GLfloat eyeoffset[2];
	std::deque<Mesh*> meshes;
};



