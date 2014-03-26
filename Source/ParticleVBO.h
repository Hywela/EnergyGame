#pragma once
#include <iostream>
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
class ParticleVBO{
private:

	GLuint  vboID,
		colorID;
	std::vector<GLfloat> vertices;
	std::vector <GLfloat> colors;
	void setVBO();
public:
	ParticleVBO();
	~ParticleVBO();
	void draw();
	void pushBack(b2Vec2 center, float angle, float radius, b2Vec3 color);
	void clear();
	void makeCircle(b2Vec2 center, float angle, float radius, b2Vec3 color);
};
