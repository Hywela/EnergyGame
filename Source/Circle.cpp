#include "Circle.h"



Circle::Circle() {

}

Circle::~Circle() {

}

void Circle::draw(b2Vec2 center, float angle, float radius, b2Vec3 color){
	vector <float> vertices;
	vector <float> colors;

	//Push central point
	vertices.push_back(0);
	vertices.push_back(0);
	colors.push_back(color.x);
	colors.push_back(color.y);
	colors.push_back(color.z);

	//Creat points for circle (fan around center)
	for (float i = 0.0; i <= 360; i += 360.0 / 30 ) {
		float thisX = (cos(i * M_PI / 180.0) * (radius)) * M2P;
		float thisY = (sin(i * M_PI / 180.0) * (radius)) * M2P;
		
		//Center
		vertices.push_back(thisX);
		vertices.push_back(thisY);
		colors.push_back(color.x);
		colors.push_back(color.y);
		colors.push_back(color.z);
	}

	//Move to position of the object
	glTranslatef(center.x * M2P, center.y * M2P, 0);
	glRotatef(angle * 180.0 / M_PI, 0, 0, 1);

	//Enable required buffers
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//Fill buffers with draw data
	glVertexPointer(2, GL_FLOAT, 0, &vertices.front());
	glColorPointer(3, GL_FLOAT, 0, &colors.front());

	//draw the Cube
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);

	//restore the state GL back
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	//Move back
	glTranslatef(-center.x * M2P, -center.y * M2P, 0);
}



