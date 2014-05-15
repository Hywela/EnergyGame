#pragma once
#include "Circle.h"



class Particle {
private:
	b2Body *body;
	b2Joint *joint;
	b2Vec3 color;
	int timeFired;

public:
	Particle(float r = -1, float g = -1, float b = -1);
	~Particle();

	void setJoint(b2Joint *joint);
	b2Joint* fire();
	void setBody(b2Body *body);
	b2Body* getBody();
	b2Vec3 getColor();
	bool onGround();
	bool isFired();
	void update();
	void setColor(float r, float g, float b);
};