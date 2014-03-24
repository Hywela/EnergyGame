#pragma once
#include "Circle.h"



class Particle {
private:
	b2Body *body;
	b2Joint *joint;
	b2Vec3 color;
	int timeFired;

public:
	Particle();
	~Particle();

	void setJoint(b2Joint *joint);
	b2Joint* fire();
	void setBody(b2Body *body);
	b2Body* getBody();
	b2Vec3 getColor();
	bool onGround();
	bool isFired();
	void update();
};