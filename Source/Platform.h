#pragma once
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <vector>
#include <iostream>
#include <Box2D\Box2D.h>
#define _USE_MATH_DEFINES // for C++
#include <cmath>

#define _USE_MATH_DEFINES // for C
#include <math.h>


using namespace std;
const float M2P = 30;
const float P2M = 1 / M2P;
class Platform
{
private:
	vector <float> colors;
public:
	Platform();
	~Platform();
	
	void draw(b2Vec2* points, b2Vec2 center, float angle);
};

