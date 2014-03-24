#pragma once
#include <vector>
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <mutex>
#include <Box2D\Box2D.h>
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#define _USE_MATH_DEFINES // for C
#include <math.h>
#include "Consts.h"
class VBO{
private:
	std::mutex accessMutex;
	GLuint  vboID,
		texcoordsID,
		normalID,
		textureid,
		indiceID;
	int     vertCount,
		indexCount,
		normCount,
		texCoordCount;
	bool inUse;
	float angle;
	int centerX, centerY;
	std::vector<GLfloat> vertices;
	std::vector <GLfloat> colors;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texCoords;
	std::vector<GLuint> indices;
	void SetVBO();
public:
	VBO();
	~VBO();
	void draw();
	void setInUse(bool isItInUse);
	void pushBack(b2Vec2* points, b2Vec2 center, float angle, b2Vec3 color);
	bool isInUse();
	void clear();
};
