#include "Platform.h"


Platform::Platform()
{
}

Platform::~Platform()
{
}
void Platform:: draw(b2Vec2* points, b2Vec2 center, float angle, b2Vec3 color){
	vector <float> vertices;
	vector <float> colors;
	
	//Create rectangle vertices
	for (int i = 0; i < 4; i++) {
		vertices.push_back(points[i].x*M2P);
		vertices.push_back(points[i].y*M2P);
		colors.push_back(color.x);
		colors.push_back(color.y);
		colors.push_back(color.z);
	}

	//Move to position of the object
	glTranslatef(center.x*M2P, center.y*M2P, 0);
	glRotatef(angle*180.0 / M_PI, 0, 0, 1);

	//Enable required buffers
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, &vertices.front());
	glColorPointer(3, GL_FLOAT, 0, &colors.front());

	//draw the Cube
	glDrawArrays(GL_QUADS, 0, vertices.size() / 2);

	//restore the state GL back
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	//Move back
	glTranslatef(-center.x*M2P, -center.y*M2P, 0);
}