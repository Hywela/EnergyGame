#pragma once
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
using namespace std;



class Box {
private:
	float x, y, z;
	float scalex, scaley;
	float r, g, b;

public:
	Box(float x, float y, float scalex = 50, float scaley = 50, float r = 1, float g = 1, float b = 1);
	~Box();

	void Render();
};