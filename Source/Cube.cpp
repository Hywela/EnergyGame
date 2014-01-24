#include "Cube.h"



Cube::Cube(float x, float y, float z, float scalex, float scaley, float scalez) {
	this->x = x;
	this->y = y;
	this->z = z;

	this->scalex = scalex;
	this->scaley = scaley;
	this->scalez = scalez;
}

Cube::~Cube() {
}

void Cube::Render() {
	float SX = scalex / 2;
	float SY = scaley / 2;
	float SZ = scalez / 2;

	//Cube:
	float vertices[] = {
		-SX, -SY,  SZ,	 SX, -SY,  SZ,	 SX,  SY,  SZ,	-SX,  SY,  SZ,	//v0, v1, v2, v3   front
		 SX, -SY,  SZ,	 SX, -SY, -SZ,	 SX,  SY, -SZ,	 SX,  SY,  SZ,  //v1, v5, v6, v2   right
		-SX,  SY,  SZ,	 SX,  SY,  SZ,	 SX,  SY, -SZ,	-SX,  SY, -SZ,  //v3, v2, v6, v7   top
		-SX, -SY,  SZ,	-SX,  SY,  SZ,	-SX,  SY, -SZ,	-SX, -SY, -SZ,  //v0, v3, v7, v4   left
		-SX, -SY,  SZ,	 SX, -SY,  SZ,	 SX, -SY, -SZ,	-SX, -SY, -SZ,  //v0, v1, v5, v4   bottom
		-SX, -SY, -SZ,	 SX, -SY, -SZ,	 SX,  SY, -SZ,	-SX,  SY, -SZ   //v4, v5, v6, v7   back
	};

	//All the vertices on the same quad share the same normal - to give us flat shading
	float normals[] = {
		 0,  0,  1,		 0,  0,  1,		 0,  0,  1,		 0,  0,  1,		//front
		 1,  0,  0,		 1,  0,  0,		 1,  0,  0,		 1,  0,  0,		//right
		 0,  1,  0,		 0,  1,  0,		 0,  1,  0,		 0,  1,  0,		//top
		-1,  0,  0,		-1,  0,  0,		-1,  0,  0,		-1,  0,  0,		//left
		 0, -1,  0,		 0, -1,  0,		 0, -1,  0,		 0, -1,  0,		//bottom
		 0,  0, -1,		 0,  0, -1,		 0,  0, -1,		 0,  0, -1		//back
	};

	//Render at position
	glTranslatef(x, y, z);

	//Tell OpenGL about our vertex and normal data
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normals);

	//draw the Cube
	glDrawArrays(GL_QUADS, 0, 24);

	//restore the state GL back
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	//Move back to camera
	glTranslatef(-x, -y, -z);
}