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



const float MAX_FORCE = 20;

class Circle {
private:


	


public:
	Circle();
	~Circle();

	void draw(b2Vec2 center, float angle,  float radius);
	

	
};