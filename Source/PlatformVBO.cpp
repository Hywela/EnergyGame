#include "PlatformVBO.h"

void PlatformVBO::draw(){
	if (vertices.size() > 0){
		setVBO();
		
		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glVertexPointer(2, GL_FLOAT, 0, 0);

		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, colorID);
		glColorPointer(3, GL_FLOAT, 0, 0);
		
		glDrawArrays(GL_QUADS, 0, vertices.size()/2);

		glDisableClientState(GL_COLOR_ARRAY);
		//restore the GL state back
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, 0); //Restore non VBO mode

	}

}

PlatformVBO::PlatformVBO() {
}
PlatformVBO::~PlatformVBO(){
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &colorID);
}

void PlatformVBO::setVBO(){
	// Vertices:
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *vertices.size(), &vertices.front(), GL_STATIC_DRAW);

	glGenBuffers(1, &colorID);
	glBindBuffer(GL_ARRAY_BUFFER, colorID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *colors.size(), &colors.front(), GL_STATIC_DRAW);

}
void PlatformVBO::pushBack(b2Vec2* points, b2Vec2 center, float angle, b2Vec3 color){
	int moveX = center.x*M2P;
	int moveY = center.y*M2P;
	if (color.x == 0 && color.y == 1){
	postions.push_back(moveX);
	postions.push_back(moveY);}
	for (int i = 0; i < 4; i++) {
		//std::cout << "c " << center.x << " " << points[i].x * M2P << " d " << points[i].y * M2P;
		vertices.push_back(points[i].x  *M2P + center.x*M2P);
		vertices.push_back(points[i].y *M2P + center.y*M2P);
		colors.push_back(color.x);
		colors.push_back(color.y);
		colors.push_back(color.z);
	}
}
void PlatformVBO::pushBackground(b2Vec2* points, b2Vec2 center,  b2Vec3 color){

	for (int i = 0; i < 4; i++) {
		//std::cout << "c " << center.x << " " << points[i].x * M2P << " d " << points[i].y * M2P;
		vertices.push_back(points[i].x );
		vertices.push_back(points[i].y );
		colors.push_back(color.x);
		colors.push_back(color.y);
		colors.push_back(color.z);
	}

}
void PlatformVBO::clear(){
	vertices.clear();
	colors.clear();
	postions.clear();
}

GLfloat *PlatformVBO::getCenter(){
	int s = postions.size();
	GLfloat *dd;
	//std::cout << " " << s;
	dd = new GLfloat[s * 2];
	for (int i = 1; i < s; i += 1){
		dd[i] = postions.at(i);
	}
	return dd;
}
int PlatformVBO::getCenterSize(){
	return postions.size();
}