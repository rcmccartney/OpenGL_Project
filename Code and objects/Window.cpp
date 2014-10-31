#include "Window.h"
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif
#include <iostream>
#include "vec-util.h"

//Need these to pass as function pointers to glut
Window *instance;

static void renderWrapper() {
	instance->render();
}
static void updateWrapper() {
	instance->update();
}
static void keyboardWrapper( unsigned char key, int x, int y ) {
	instance->keyboard(key, x, y);
}
static void dropMouseWrapper(int button, int state, int x, int y) {
	instance->dropMouse(button, state, x, y);
}
static void dragMouseWrapper(int x, int y) {
	instance->dragMouse(x,y);
}
static void reshapeWrapper(int w, int h) {
	instance->reshape(w, h);
}

Window::Window(Shader *aShader, int argc, char* argv[] ) :
	shader(aShader)  {

	eyeoffset[0] = 0; eyeoffset[1] = 0;
    window_size[0] = INITIAL_WIDTH;
    window_size[1] = INITIAL_HEIGHT;
	
	for(int i = 0; i < 4; i++) 
		ambientcolor[i] = 0.2f; 

	//camera setup
	up[0] = 0.0f; up[1] = 1.0f; up[2] = 0.0f;
	eye[0] = 0.0f; eye[1] = 1.0f; eye[2] = 3.0f;
	look[0] = eye[0]; look[1] = eye[1]-.5f; look[2] = eye[2];  //start looking at origin but slightly up
	setUpFrustrum();

	setupCallback();
}

void Window::setupCallback() {
	
	::instance = this;
	glutIdleFunc(&updateWrapper);
    glutDisplayFunc(&renderWrapper);
    glutReshapeFunc(&reshapeWrapper);
    glutMotionFunc(&dragMouseWrapper);
    glutMouseFunc(&dropMouseWrapper);
    glutKeyboardFunc(&keyboardWrapper);
}

void Window::addMesh(Mesh *aMesh) {
	meshes.push_back(aMesh);
}

void Window::update() {
    int milliseconds = glutGet(GLUT_ELAPSED_TIME);
    GLfloat seconds = (GLfloat) milliseconds * (1.0f/1000.0f);
	for(int i = 0; i < meshes.size(); i++) {
		meshes[i]->update_mesh(seconds);
	}
	glutPostRedisplay();
}

void Window::keyboard( unsigned char key, int x, int y ) {
    switch( key ) {
        case '1': setUpOrtho(); break;
		case '2': setUpFrustrum(); break;
		//rotations 
		case 'x': rotateX(ANGLEINC); break;
        case 'y': rotateY(ANGLEINC); break;
        case 'z': rotateZ(ANGLEINC); break;  
        case 'X': rotateX(-ANGLEINC); break;
		case 'Y': rotateY(-ANGLEINC); break;
		case 'Z': rotateZ(-ANGLEINC); break;
		case 'r': case 'R': //Press R to reload a shader
			shader->updateShader(); 
			break;
		case 'a': case 'A': //Press a to move left
			moveLeft(); 
			break;
		case 'd': case 'D': //Press d to move right
			moveRight(); 
			break;
		case 'w': case 'W': //Press w to move up
			moveUp(); 
			break;
		case 's': case 'S': //Press s to move down
			moveDown(); 
			break;
		case '+': changeAmbience(AMB_INC); break;
        case '=': changeAmbience(-AMB_INC); break;
		case 033:  // Escape key
        case 'q': case 'Q':
            exit( 0 ); 
			break;
    } 
    glutPostRedisplay();
}

void Window::changeAmbience(float amount) {
	for(int i =0; i < 4; i++)
		ambientcolor[i] += amount;
}

void Window::rotateX(float amount) {
	meshes[0]->rotateX(amount);
}

void Window::rotateY(float amount) {
	meshes[0]->rotateY(amount);
}

void Window::rotateZ(float amount) {
	meshes[0]->rotateZ(amount);
}

void Window::dropMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		eyeoffset[0] = 0; eyeoffset[1] = 0;
		setUpCamera();
    }
}

void Window::dragMouse(int x, int y) {
    float w = (float) window_size[0];
    float h = (float) window_size[1];
    eyeoffset[0] = (float) x/w - 0.5f;
    eyeoffset[1] = -(float) y/h + 0.5f;
	setUpCamera();
}

void Window::moveLeft() {
		float left[3]; 
        vec_cross(left, look, up);
        vec_normalize(left);
		eye[0] += (left[0] * STEPSIZE);
		eye[1] += (left[1] * STEPSIZE);
		eye[2] += (left[2] * STEPSIZE);
		setUpCamera();
}

void Window::moveRight() {
		float right[3]; 
        vec_cross(right, up, look);
        vec_normalize(right);
		eye[0] += (right[0] * STEPSIZE);
		eye[1] += (right[1] * STEPSIZE);
		eye[2] += (right[2] * STEPSIZE);
		setUpCamera();
}

void Window::moveUp() {
	
	float temp[3] = { look[0], look[1], look[2] };
	vec_normalize(temp);
	eye[0] -= (temp[0] * STEPSIZE);
	eye[1] -= (temp[1] * STEPSIZE);
	eye[2] -= (temp[2] * STEPSIZE);
	setUpCamera();
}

void Window::moveDown() {

	float temp[3] = { look[0], look[1], look[2] };
	vec_normalize(temp);
	eye[0] += (temp[0] * STEPSIZE);
	eye[1] += (temp[1] * STEPSIZE);
	eye[2] += (temp[2] * STEPSIZE);
	setUpCamera();
}

void Window::setUpCamera() {
	//camera setup
	GLfloat eyeMinusLook[3] = { look[0], look[1], look[2] };
	GLfloat eyePlusOffset[3] = { eye[0]+eyeoffset[0], eye[1]+eyeoffset[1], eye[2] };
	vec_normalize( eyeMinusLook );
	float u[3], v[3];
	vec_cross(u, up, eyeMinusLook);
	vec_normalize(u);
	vec_cross(v, eyeMinusLook, u);
	float cam_mat[] = { u[0], v[0], eyeMinusLook[0], 0,
						u[1], v[1], eyeMinusLook[1], 0,
						u[2], v[2], eyeMinusLook[2], 0, 
						-dot(u, eyePlusOffset), -dot(v, eyePlusOffset), -dot(eyeMinusLook, eyePlusOffset), 1 };
	for(int i = 0; i < 16; i++) {
		v_matrix[i] = cam_mat[i];
	}
}

/**
 * This functions sets up the view and projection parameter for a frustrum
 * projection of the scene. See the assignment description for the values
 * for the projection parameters.
 *
 */
void Window::setUpFrustrum() {

	setUpCamera();
	//set up perspective
	float per_mat[] = { 2*PROJECTION_NEAR/(PROJECTION_RIGHT-PROJECTION_LEFT), 0, 0, 0,
						0, 2*PROJECTION_NEAR/(PROJECTION_TOP-PROJECTION_BOTTOM), 0, 0,
						(PROJECTION_RIGHT+PROJECTION_LEFT)/(PROJECTION_RIGHT-PROJECTION_LEFT), (PROJECTION_TOP+PROJECTION_BOTTOM)/(PROJECTION_TOP-PROJECTION_BOTTOM),
										-(PROJECTION_FAR+PROJECTION_NEAR)/(PROJECTION_FAR-PROJECTION_NEAR), -1, 
						0, 0, -2*PROJECTION_FAR*PROJECTION_NEAR/(PROJECTION_FAR-PROJECTION_NEAR), 0 };
	for(int i = 0; i < 16; i++) {
		p_matrix[i] = per_mat[i];
	}
}

/**
 * This functions sets up the view and projection parameter for an orthographic
 * projection of the scene. See the assignment description for the values
 * for the projection parameters.
 *
 */
void Window::setUpOrtho() {

	setUpCamera();
	//set up perspective
	float per_mat[] = { 2/(PROJECTION_RIGHT-PROJECTION_LEFT), 0, 0, 0, 
						0, 2/(PROJECTION_TOP-PROJECTION_BOTTOM), 0, 0,
						0, 0, -2/(PROJECTION_FAR-PROJECTION_NEAR), 0,
						-(PROJECTION_RIGHT+PROJECTION_LEFT)/(PROJECTION_RIGHT-PROJECTION_LEFT), -(PROJECTION_TOP+PROJECTION_BOTTOM)/(PROJECTION_TOP-PROJECTION_BOTTOM), 
								-(PROJECTION_FAR+PROJECTION_NEAR)/(PROJECTION_FAR-PROJECTION_NEAR), 1 };
	for(int i = 0; i < 16; i++) {
		p_matrix[i] = per_mat[i];
	}
}

void Window::reshape(int w, int h) {

	if (w != 0 && h != 0) {
		window_size[0] = w;
		window_size[1] = h;
		PROJECTION_LEFT = (w/h)*PROJECTION_BOTTOM;
		PROJECTION_RIGHT = (w/h)*PROJECTION_TOP;
		glViewport(0, 0, w, h);
		setUpFrustrum();
	}
}

void Window::render_mesh() {
	for(int i = 0; i < meshes.size(); i++) {
		//change the model transformations
		glUniformMatrix4fv(shader->m_matrix, 1, GL_FALSE, meshes[i]->get_m_matrix() );
		glBindTexture(GL_TEXTURE_2D, meshes[i]->texture);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->vertex_buffer);
		glVertexAttribPointer(shader->position, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
								(void*) offsetof(struct vertex, position) );
		glVertexAttribPointer(shader->normal, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
								(void*) offsetof(struct vertex, normal) );
		glVertexAttribPointer(shader->texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
								(void*) offsetof(struct vertex, texcoord) );
		glVertexAttribPointer(shader->shininess, 1, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
								(void*) offsetof(struct vertex, shininess) );
		glVertexAttribPointer( shader->specular, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(struct vertex),
								(void*) offsetof(struct vertex, specular) );

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->element_buffer);
		glDrawElements(GL_TRIANGLES, meshes[i]->element_count, GL_UNSIGNED_SHORT, (void*) 0);
	}
}

void Window::render() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader->program);
    glActiveTexture(GL_TEXTURE0);
    //set uniform vars
	glUniform1i(shader->texture, 0);
    glUniformMatrix4fv(shader->p_matrix, 1, GL_FALSE, p_matrix);
    glUniformMatrix4fv(shader->v_matrix, 1, GL_FALSE, v_matrix);
	//set lighting
	glUniform4fv(shader->lightPos, 1 , lightpos);
    glUniform4fv(shader->specCol, 1 , specularcolor);
    glUniform4fv(shader->diffuseCol, 1 , diffusecolor);
	glUniform4fv(shader->light_ambient, 1 , ambientcolor);
	//set attrib vars
    glEnableVertexAttribArray(shader->position);
    glEnableVertexAttribArray(shader->normal);
    glEnableVertexAttribArray(shader->texcoord);
    glEnableVertexAttribArray(shader->shininess);
    glEnableVertexAttribArray(shader->specular);

    render_mesh();

    glDisableVertexAttribArray(shader->position);
    glDisableVertexAttribArray(shader->normal);
    glDisableVertexAttribArray(shader->texcoord);
    glDisableVertexAttribArray(shader->shininess);
    glDisableVertexAttribArray(shader->specular);
    glutSwapBuffers();
}

Window::~Window(void)
{
}
