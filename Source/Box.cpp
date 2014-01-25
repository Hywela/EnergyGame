#include "Box.h"



Box::Box(float x, float y, float scalex, float scaley, float r, float g, float b) {
	this->x = x;
	this->y = y;
	this->z = 0;

	this->r = r;
	this->g = g;
	this->b = b;

	this->scalex = scalex;
	this->scaley = scaley;
}

Box::~Box() {
}

void Box::Render() {
	//Cube:
	float vertices[] = {
		x, y, z,
		x + scalex, y, z,
		x + scalex, y + scaley,
		z,	x, y + scaley, z
	};

	float colors[] = {
		r, g, b,	r, g, b,	r, g, b,	r, g, b
	};

	//Render at position
	glTranslatef(x, y, z);

	//Tell OpenGL about our vertex and normal data
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, colors);
	
	//draw the Cube
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	//restore the state GL back
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	//Move back to camera
	glTranslatef(-x, -y, -z);
}