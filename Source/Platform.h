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
#include "Consts.h"
#include "Texture.h"
using namespace std;



class Platform {
private:

public:
	Platform();
	~Platform();
	
	void draw(b2Vec2* points, b2Vec2 center, float angle, b2Vec3 color = b2Vec3(1, 1, 1));
};

