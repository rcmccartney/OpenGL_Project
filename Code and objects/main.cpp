#include <GL/glew.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#include <iostream>
#include "vec-util.h"
#include "Window.h"
#include "Flag.h"
#include "Flagpole.h"
#include "Ball.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Cube.h"
#include "objMesh.h"
#include "GROUND.h"
#include "Wall.h"

static GLubyte blue[4] = {0, 0, 255, 255};
static GLubyte red[4] = {255, 0, 0, 255};
static GLubyte seagreen[4] = {60,179,113,0};
static GLubyte dRed[4] = {204,0,0,0};
static GLubyte brown[4] = {102, 51, 0, 0};
static GLubyte white[4] = {255, 255, 255, 255};
static GLubyte black[4] = {0, 0, 0, 0};
static GLubyte grey[4] = {192, 192, 192, 0};

static float floor_lo[3] = {-3, -1, -5};
static float floor_hi[3] = {3, -1, 5};
static float wall1_lo[3] = {-3, -1, -2};
static float wall1_hi[3] = {3, 6, -2};
static float wall2_lo[3] = {-3, -1, 2};
static float wall2_hi[3] = {-3, 6, -2};
static float wall3_lo[3] = {3, -1, -2};
static float wall3_hi[3] = {3, 6, 2};

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(INITIAL_WIDTH, INITIAL_HEIGHT);
    glutCreateWindow("CG Final Project");
#ifndef __APPLE__
    glewInit();
#endif
    if (!GLEW_VERSION_2_0) {
        fprintf(stderr, "OpenGL 2.0 not available\n");
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    glClearColor(.9f, 1.0f, 1.0f, 1.0f);
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); //used for wire frames

	Shader myShader("v.glsl", "f.glsl");
	Window myWindow(&myShader, argc, argv);

	float translation[3] = { -1, 2.5, -1.5}; 
	Flag *flag = new Flag("american.jpg");
	Flagpole *flagpole = new Flagpole("InteriorWoodFlooringMedium_01.jpg");
	flagpole->rotateY(90);
	flag->rotateZ(-90);
	flag->rotateX(-10);
	flagpole->rotateZ(90);
	flag->rotateY(20);
	flagpole->rotateY(20);
	flag->translate(translation);
	flagpole->translate(translation);
	myWindow.addMesh(flag);
	myWindow.addMesh(flagpole);
	
	translation[0] = 1.6; translation[1] = -0.2; translation[2] = -1;
	float scale[3] = {1.5,1.5,1.5};
	objMesh *couch = new objMesh("leather.jpg", "Couch.obj", true, true, 10, brown);
	objMesh *couch2 = new objMesh("leather.jpg", "Couch2.obj", true, true, 100, brown);
	couch->scale(scale); 	couch2->scale(scale);  
	couch->translate(translation); 	
	translation[0] = 1.1; translation[1] = 0; translation[2] = -1.3; 
	couch2->translate(translation);
	myWindow.addMesh(couch); myWindow.addMesh(couch2);

	translation[0] = 0; translation[1] = -1; translation[2] = 0; 
	objMesh *cowhide = new objMesh("fur.jpg", "cowhide.obj", true, true, 100, black);
	cowhide->translate(translation);
	myWindow.addMesh( cowhide );
	
	translation[0] = 2; translation[1] = 0.3; translation[2] = -1; 
	objMesh *lamp = new objMesh("metallic.jpg", "Light.obj", true, true, 1, grey);
	lamp->rotateY(-60);
	lamp->translate(translation);
	myWindow.addMesh(lamp);

	translation[0] = -2; translation[1] = -1; translation[2] = -1; 
	objMesh *table = new objMesh("wood.jpg", "Coffee.obj", true, true, 100, brown);
	table->translate(translation);
	myWindow.addMesh(table);

	translation[0] = 0; translation[1] = -1; translation[2] = .3; 
	objMesh *books = new objMesh("red.jpg", "Books.obj", true, true, 100, red);
	books->translate(translation);
	myWindow.addMesh(books);

	translation[0] = -2; translation[1] = -0.4; translation[2] = -1; 
	objMesh *twig = new objMesh("", "Twig.obj", true, true, 10, dRed);
	twig->translate(translation);
	myWindow.addMesh(twig);

	translation[0] = -2; translation[1] = -0.4; translation[2] = -0.7; 
	objMesh *glass = new objMesh("glass.jpg", "Water.obj", true, true, 40, blue);
	glass->translate(translation);
	myWindow.addMesh(glass);

	translation[0] = -0.5; translation[1] = -0.2; translation[2] = -1; 
	objMesh *cushion = new objMesh("stripes.jpg", "Cushion.obj", true, true, 10, white);
	cushion->scale(scale);
	cushion->rotateX(90);
	cushion->translate(translation);
	myWindow.addMesh(cushion);

	translation[0] = 0.9; 
	objMesh *cushion2 = new objMesh("stripes.jpg", "Cushion.obj", true, true, 10, white);
	cushion2->scale(scale);
	cushion2->rotateX(90);
	cushion2->translate(translation);
	myWindow.addMesh(cushion2);

	myWindow.addMesh(new Wall("brick-background.jpg", 8, 4, 100, red, wall1_lo, wall1_hi) );
	myWindow.addMesh(new Wall("white.jpg", 1, 1, 40, white, wall2_lo, wall2_hi) );
	myWindow.addMesh(new Wall("white.jpg", 1, 1, 40, white, wall3_lo, wall3_hi) );
	myWindow.addMesh(new GROUND("InteriorWoodFlooringMedium_01.jpg", 
					 8, 8, 200, white, floor_lo, floor_hi) );

	glutMainLoop();
}
