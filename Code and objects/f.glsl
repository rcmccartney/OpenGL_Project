#version 110

uniform sampler2D texture;
uniform vec4 specularColor, diffuseColor, light_ambient;
//add a little light instead of a ray tracer with ambient

varying vec3 fPosition, fNormal, lPos;
varying vec2 fTexcoord;
varying float specularExponent;
varying vec4 fSpecularCol;

float warp_diffuse(float d)
{
    return d * 0.5 + 0.5;
}

float cel(float d)
{
	d = d * 0.5 + 0.5;
    return smoothstep(0.35, 0.37, d) * 0.4 + smoothstep(0.70, 0.72, d) * 0.6;
}

void main()
{
	// calculate your vectors
    vec3 L = normalize(lPos - fPosition);
    vec3 norm = normalize(fNormal);
    vec3 eye = normalize(-fPosition);  //neg or not??
    vec3 reflection = reflect(L, norm);

    vec4 textureColor = texture2D(texture, fTexcoord);
    vec4 diffuse_factor = max(dot(norm, L), 0.0)*diffuseColor + light_ambient;
    vec4 specular_factor = max(pow(dot(reflection, eye), specularExponent), 0.0) * specularColor;
    
    gl_FragColor = specular_factor*fSpecularCol + diffuse_factor*textureColor;
}
