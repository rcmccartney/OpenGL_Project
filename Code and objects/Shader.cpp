#include "Shader.h"
#include <iostream>
#include <math.h>
#include <SOIL.h>

Shader::Shader(char *aVert, char *aFrag) :
	vert(aVert),
	frag(aFrag) 
{
	vertex_shader = 0;
	fragment_shader = 0;
	program = compileShader(vertex_shader, fragment_shader);
	if (!program) {
		std::cerr << "Failed to initally load shader" << std::endl;
        exit( 1 );
    }
	enactShader(vertex_shader, fragment_shader, program);
}

void Shader::show_info_log( GLuint object,
							PFNGLGETSHADERIVPROC glGet_IVFunction,
							PFNGLGETSHADERINFOLOGPROC glGet__InfoLog ) {
    GLint log_length;
    char *log;
    glGet_IVFunction(object, GL_INFO_LOG_LENGTH, &log_length);
    log = (char *) malloc(log_length);
    glGet__InfoLog(object, log_length, NULL, log);
    fprintf(stderr, "%s", log);
    free(log);
}

/**
* This method reads a shader file and stores it as an array of char
*
*/
char* Shader::textFileRead(char *fn) {
	FILE *fp;
	char *content = NULL;
	int count=0;
	if (fn != NULL) {
		fp = fopen(fn,"rt");
		if (fp != NULL) {
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count > 0) {
				content = (char *) malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	else {
		fprintf(stderr, "Error reading %s\n", fn);
	}
	return content;
}

GLuint Shader::compileShader(GLuint &vShader, GLuint &fShader) {

	// Read in shader source
	char *vs = NULL, *fs = NULL;
	// Create the shader 
	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLint shader_ok, program_ok;
	// Read in shader source
	vs = textFileRead(vert);
	if (!vs) {
		fprintf(stderr, "Error reading vertex shader file %s\n", vert);
		return 0;
	}
	fs = textFileRead(frag);
	if (!fs) {
		fprintf(stderr, "Error reading fragment shader file %s\n", frag);
		return 0;
	}
	glShaderSource(vShader, 1, (const GLchar**) &vs, NULL);
	glShaderSource(fShader, 1, (const GLchar**) &fs, NULL);
	free(vs);free(fs);
	
	// Compile the shaders
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok) {
        fprintf(stderr, "Failed to compile %s:\n", vert);
        show_info_log(vShader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(vShader);
        return 0;
    }
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok) {
        fprintf(stderr, "Failed to compile %s:\n", frag);
        show_info_log(fShader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(fShader);
        return 0;
    }
	// Create the program -- attaching your shaders
	GLuint program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	// Link the program
	glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
    if (!program_ok) {
        fprintf(stderr, "Failed to link shader program:\n");
        show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
        return 0;
    }
	return program;
}

/*
 * Store the new shader permanently, used to modify shaders while program is running
 * Need to modify this if you are going to add other uniforms or attributes
  */
void Shader::enactShader(GLuint vShader, GLuint fShader, GLuint aProgram) {
    
	vertex_shader = vShader;
    fragment_shader = fShader;
    program = aProgram;
	//uniforms
	texture = glGetUniformLocation(program, "texture");
	p_matrix = glGetUniformLocation(program, "p_matrix");
    v_matrix = glGetUniformLocation(program, "v_matrix");
	m_matrix = glGetUniformLocation(program, "m_matrix");
	// Here's the variables for lighting
    lightPos = glGetUniformLocation( program , "lightPosition" );
	specCol = glGetUniformLocation( program , "specularColor" );
    diffuseCol = glGetUniformLocation( program , "diffuseColor" );
	light_ambient = glGetUniformLocation( program , "light_ambient" );
	//attributes
	position = glGetAttribLocation(program, "position");
	normal = glGetAttribLocation(program, "normal");
    texcoord = glGetAttribLocation(program, "texcoord");
    shininess = glGetAttribLocation(program, "shininess");
    specular = glGetAttribLocation(program, "specular");
}

int Shader::remakeShader(GLuint &vShader, GLuint &fShader, GLuint &aProgram) {

    aProgram = compileShader(vShader, fShader);
    if (aProgram == 0) {
		std::cerr << "Failed to compile shader into a program, keeping old shader." << std::endl;
        return 0;
	}
	else {
		deleteShader();
		enactShader(vShader, fShader, aProgram);
		return 1;
	}
}

void Shader::deleteShader() {
	if (program) {
		std::cout << "HERE!!" << std::endl;
		glDetachShader(program, vertex_shader);
		glDetachShader(program, fragment_shader);
		glDeleteProgram(program);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}
}

void Shader::updateShader() {

	std::cout << "Reloading shader program" << std::endl;
    GLuint vShader, fShader, aProgram;
	remakeShader(vShader, fShader, aProgram);
}

Shader::~Shader(void) {
	this->deleteShader();
}

