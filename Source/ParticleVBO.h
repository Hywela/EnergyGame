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
#include "Shader.h"

/**		--ParticleVBO--
- ParticleVBO handles all drawing of circle types,
can also be used as a storce for ligths(center).
											-kristoffer
**/
class ParticleVBO{
private:
	GLuint  vboID, colorID;
	std::vector<GLfloat> vertices;
	std::vector <GLfloat> colors;
	std::vector <GLfloat> postions;
	b2Vec2 center;
	//Private Setter
	void setVBO();
public:
	ParticleVBO();
	~ParticleVBO();
	//Draw
	void draw();
	//Setters
	void setCenter(b2Vec2 center);
	void pushBackCenter(b2Vec2 pcenter);
	void pushBack(b2Vec2 pCenter, float angle, float radius, b2Vec3 color);
	//Clears
	void clear();
	//Getters
	int getCenterSize();
	GLfloat *getCenter();
	b2Vec2 getMainCenter();
};
