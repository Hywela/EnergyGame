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
	glGenBuffers(1, &vboID);
	glGenBuffers(1, &colorID);

}
PlatformVBO::~PlatformVBO(){
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &colorID);
}

void PlatformVBO::setVBO(){
	// Vertices:
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *vertices.size(), &vertices.front(), GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, colorID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *colors.size(), &colors.front(), GL_STATIC_DRAW);

}
void PlatformVBO::pushBack(b2Vec2* points, b2Vec2 center, float angle, b2Vec3 color){
	int moveX = center.x*M2P;
	int moveY = center.y*M2P;
	
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
void PlatformVBO::pushBackLigthPostionLit(b2Vec2 center){
	int moveX = center.x*M2P;
	int moveY = center.y*M2P;
	litPostions.push_back(moveX);
	litPostions.push_back(moveY);
}
void PlatformVBO::pushBackLigthPostionUnlit(b2Vec2 center){
	int moveX = center.x*M2P;
	int moveY = center.y*M2P;
	unlitPostions.push_back(moveX);
	unlitPostions.push_back(moveY);
}

void PlatformVBO::clear(){
	vertices.clear();
	colors.clear();
	if (litPostions.size() > 0)
	litPostions.clear();
	if (unlitPostions.size() > 0)
	unlitPostions.clear();
}

GLfloat *PlatformVBO::getCenterUnlit( ){


	int s = unlitPostions.size();
	GLfloat *dd;
	//std::cout << " " << s;
	dd = new GLfloat[s * 2];
	for (int i = 0; i < s; i += 1){
		dd[i] = unlitPostions.at(i);
	}
	
	return dd;
}
GLfloat *PlatformVBO::getCenterLit(){


int s = litPostions.size();
	GLfloat *dd;
	//std::cout << " " << s;
	dd = new GLfloat[s * 2];
	for (int i = 0; i < s; i += 1){
		dd[i] = litPostions.at(i);
	}

	return dd;
}
int PlatformVBO::getCenterLitSize(){


	return litPostions.size();
}
int PlatformVBO::getCenterUnlitSize(){


	return unlitPostions.size();
}