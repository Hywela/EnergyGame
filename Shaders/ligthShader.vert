#version 120

attribute vec3 vertex;
attribute vec3 color;
attribute vec2 texCoord;
attribute vec3 normal;

uniform mat4 transform;

varying vec3 shared_color;
varying vec2 shared_texCoord;
varying vec3 shared_normal;

void main() {
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;  
	
	//Send data to fragment shader
	shared_color = color;
	shared_texCoord = texCoord;
	shared_normal = (transform * vec4(normal, 0.0)).xyz;
}