#pragma once
#include <iostream>
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <vector>
using namespace std;



class Sphere {
private:
	float x, y, z;
	float radius;
	vector <float> vertices;
	vector <float> normals;

public:
	Sphere(float x, float y, float z, float radius = 1, float degreeStep = 45, float heightStep = 0.1);
	~Sphere();

	void Render();
};
