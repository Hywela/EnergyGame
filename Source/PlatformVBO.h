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
/**		--PlatformVBO--
- PlatformVBO handles all drawing of square types
								-kristoffer 
**/
class PlatformVBO{
private:
	GLuint  vboID, colorID;
	std::vector<GLfloat> vertices;
	std::vector <GLfloat> colors;
	std::vector <GLfloat> postions;
	void setVBO();
public:
	PlatformVBO();
	~PlatformVBO();

	void draw(); 
	//Setters
	void pushBack(b2Vec2* points, b2Vec2 center, float angle, b2Vec3 color);
	void pushBackground(b2Vec2* points, b2Vec2 center, b2Vec3 color);
	void pushBackLigthPostion(b2Vec2 center);
	void setCenter(b2Vec2 center);
	//Clear
	void clear();
	//Getters
	int getCenterSize();
	GLfloat *getCenter();
	
};
