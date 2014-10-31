#version 110

uniform mat4 p_matrix, m_matrix, v_matrix;
// Light position is given in world space
uniform vec4 lightPosition;

attribute vec3 position, normal;
attribute vec2 texcoord;
attribute float shininess;
attribute vec4 specular;

varying vec3 fPosition, fNormal, lPos;
varying vec2 fTexcoord;
varying float specularExponent;
varying vec4 fSpecularCol;

void main()
{
	mat4 mv_matrix = v_matrix * m_matrix;
    vec4 eye_position = mv_matrix * vec4(position, 1.0);
    fPosition = eye_position.xyz;
	lPos = (v_matrix * lightPosition).xyz;
    fNormal = (mv_matrix * vec4(normal, 0.0)).xyz;
	
	//fTexcoord = texcoord;
	//inverts the image since openGL reads in last row of pixels first
	fTexcoord = vec2(1.0-texcoord.x, 1.0-texcoord.y);

    specularExponent = shininess;
    fSpecularCol = specular;
	gl_Position = p_matrix * eye_position;
}
