#include "VBO.h"

void VBO::draw()
{
	if (vertices.size() > 0){
	//Move to position of the object
	glTranslatef(centerX * M2P, centerY * M2P, 0);
	glRotatef(angle * 180.0 / M_PI, 0, 0, 1);

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
	glTranslatef(-centerX * M2P, -centerY * M2P, 0);
	}
};

VBO::VBO() :inUse(false), angle(0), centerX(0), centerY(0)
{

}
VBO::~VBO()
{
	glDeleteBuffers(1, &vboID);
//	glDeleteBuffers(1, &normalID);
//	glDeleteBuffers(1, &texcoordsID);
//	glDeleteBuffers(1, &indiceID);
}

void VBO::SetVBO()
{
	// Vertices:
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *vertices.size() / 3, &vertices, GL_STATIC_DRAW);

/*	// Normals:
	glGenBuffers(1, &normalID);
	glBindBuffer(GL_ARRAY_BUFFER, normalID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *normals.size() / 3, &normals, GL_STATIC_DRAW);

	// Texture coordinates:
	glGenBuffers(1, &texcoordsID);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordsID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *texCoords.size() / 2, &texCoords, GL_STATIC_DRAW);
	*/
	vertices.clear();
//	normals.clear();
	//texCoords.clear();
}
void VBO::pushBack(b2Vec2* points, b2Vec2 center, float angle, b2Vec3 color){
	this->angle = angle;
	this->centerX = center.x;
	this->centerX = center.y;
	for (int i = 0; i < 4; i++) {
		vertices.push_back(points[i].x * M2P);
		vertices.push_back(points[i].y * M2P);
		colors.push_back(color.x);
		colors.push_back(color.y);
		colors.push_back(color.z);
	}

}
void VBO::setInUse(bool isItInUse){
	std::lock_guard<std::mutex> locker(accessMutex);
	inUse = isItInUse;
}
bool VBO::isInUse(){
	return inUse;
}
void VBO::clear(){
	vertices.clear();
	colors.clear();
}