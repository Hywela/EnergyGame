#pragma once
#include <Box2D\Box2D.h>
#include "Circle.h"

class RenderData {
private:
	int type;
	
	b2Vec2 center;
	float angle;
	float radius;
	b2Vec3 color;

public:
	b2Vec2 points[4];
	RenderData();
	RenderData(int type, b2Vec2 center, float angle, float radius, b2Vec3 color);
	RenderData(int type);
	RenderData(int type, b2Vec2 xy[4], b2Vec2 center ,float angle , b2Vec3 color);
	void test();
	void circle();
	int getType();
	int getX();
	int getY();
	b2Vec2  getCenter();
	b2Vec2  getXY();
	float getAngle();
	b2Vec3  getColor();
	float getRadius();
	~RenderData();
};

