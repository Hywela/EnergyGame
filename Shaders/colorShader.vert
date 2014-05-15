#version 120

attribute vec3 vertex;
attribute vec3 color;

varying vec3 shared_color;

void main() {
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;  
	
	//Send data to fragment shader
	shared_color = color;
}