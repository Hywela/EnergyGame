#version 120

attribute vec3 vertex;
attribute vec3 color;
attribute vec2 texCoord;

varying vec3 shared_color;
varying vec2 shared_texCoord;

void main() {
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;  
	//Send data to fragment shader
	shared_color = color;
	shared_texCoord = texCoord;
}