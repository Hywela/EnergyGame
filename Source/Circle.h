#pragma once
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <vector>
#include <iostream>
using namespace std;



const float MAX_FORCE = 20;

class Circle {
private:
	float x, y, z;
	float radius;
	float r, g, b;
	vector <float> vertices;
	vector <float> colors;
	float forcex, forcey;
	float distx, disty;
	float movex, movey;
	bool patrol;
	float pathxStart, pathxEnd, pathyStart, pathyEnd;

public:
	Circle(float x, float y, float radius = 50, float r = 1, float g = 1, float b = 1);
	~Circle();

	void Render();
	void MoveDistance(int distx, int disty);
	void MoveTo(int x, int y);
	void Update();
	bool HasTarget();
	bool IsMoving();
	void SetPatrol(bool state);
	void Move(float speedx, float speedy);
};