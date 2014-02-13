#include "Circle.h"
int ground = 0;
const float M2P = 30;
const float P2M = 1 / M2P;
Circle::Circle() {


	
}

Circle::~Circle() {
}

void Circle::draw( b2Vec2 center, float angle, float radius){
	vector <float> vertices;
	vertices.clear();

	vertices.push_back(0);
	vertices.push_back(0);
	for (float i = 0.0; i <= 360; i += 360.0/30 ) {
		float thisX = (cos(i*M_PI / 180.0)* (radius))*M2P;
		float thisY = (sin(i*M_PI / 180.0)*(radius))*M2P;
		

		//Center
		vertices.push_back(thisX);
		vertices.push_back(thisY);
	
		
	}

	
	glTranslatef(center.x*M2P, center.y*M2P, 0);
	glRotatef(angle*180.0 / M_PI, 0, 0, 1);

	



	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &vertices.front());
	
	

	//draw the Cube
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size()/2);

	//restore the state GL back
	
	glDisableClientState(GL_VERTEX_ARRAY);




	glTranslatef(-center.x*M2P, -center.y*M2P, 0);

}



