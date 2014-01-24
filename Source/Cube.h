#pragma once
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
using namespace std;



class Cube {
private:
	float x, y, z;
	float scalex, scaley, scalez;

public:
	Cube(float x, float y, float z, float scalex = 1, float scaley = 1, float scalez = 1);
	~Cube();

	void Render();
};