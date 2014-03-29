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
class ParticleVBO{
private:

	GLuint  vboID,
		colorID;
	std::vector<GLfloat> vertices;
	std::vector <GLfloat> colors;
	std::vector <GLfloat> postions;
	void setVBO();
	b2Vec2 center;
public:
	ParticleVBO();
	~ParticleVBO();
	void draw();
	void setCenter(b2Vec2 center);
//	b2Vec2 getCenter();
	void pushBack(b2Vec2 pCenter, float angle, float radius, b2Vec3 color);
	void clear();
	void makeCircle(b2Vec2 center, float angle, float radius, b2Vec3 color);
	void setUniforms(GLint lightColor,
		GLint mUniformscreenHeight,
		GLint lightAttenuation,
		GLint radius,
		GLint lightpos, int screenHeight, Shader *sh);
	int getCenterSize();
	GLfloat *getCenter();
	b2Vec2 getMainCenter();
};
